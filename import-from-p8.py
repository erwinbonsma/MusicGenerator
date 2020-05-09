from argparse import ArgumentParser

tab = "    "
notes = ["C", "Cs", "D", "Ds", "E", "F", "Fs", "G", "Gs", "A", "As", "B"]
waves = ["TRIANGLE", "TILTED_SAW", "SAW", "SQUARE", "PULSE", "ORGAN", "NOISE", "PHASER"]
effects = ["NONE", "SLIDE", "VIBRATO", "DROP", "FADE_IN", "FADE_OUT", "ARPEGGIO", "ARPEGGIO_FAST"]

parser = ArgumentParser()
parser.add_argument(
    "filename", help="PICO-8 source file"
)
parser.add_argument(
    "--postfix", default=""
)
parser.add_argument(
    "--comments", action='store_true'
)
args = parser.parse_args()
postfix = args.postfix

def numval(line, pos, num_chars):
    return int(line[pos:pos+num_chars], 16)

def warning(msg):
    print("// WARNING: %s" % (msg))

def note_from_spec(spec):
    volume = numval(spec, 3, 1)
    if volume > 0:
        # Note: Adding one to volume to handle range mismatch: PICO-8 range is [0, 7], Gamebuino
        # range is [0, 8]. This means that volume 1 will never be used. This could be re-introduced
        # via manual post-processing.
        volume += 1
    pitch = numval(spec, 0, 2) + 24
    wave_num = numval(spec, 2, 1)
    if wave_num < 8:
        wave = waves[wave_num]
    else:
        wave = "CUSTOM{0}".format(wave_num - 8)
    effect = effects[numval(spec, 4, 1)]
    return Note(pitch, volume, wave, effect)

def adapt_custom_note(source_note, modifier_note):
    """
    Adapts a note from a custom instrument for use in another SFX. source_note is the note as specified in
    the custom SFX, and modifier_note is the note in target SFX where the custom note should be played.
    """
    pitch = source_note.pitch + (modifier_note.pitch - 48)

    volume = int(source_note.volume * modifier_note.volume / 8)
    if source_note.volume * modifier_note.volume > 0:
        # Ensure that notes with volume never become fully silent
        volume = max(1, volume)
    return Note(pitch, volume, source_note.wave, source_note.effect)

class Note:
    def __init__(self, pitch, volume, wave, effect):
        self.pitch = pitch
        self.volume = volume
        self.wave = wave
        self.effect = effect

    def custom_wave(self):
        return self.wave.startswith("CUSTOM")

    def custom_sfx_id(self):
        return int(self.wave[6:])

    def print(self, part_of_arpeggio):
        if self.volume == 0:
            if part_of_arpeggio:
                print("%sNoteSpec { .note=Note::%s%d, .vol=0, .wav=WaveForm::NONE, .fx=Effect::NONE }," %
                    (tab, notes[self.pitch % 12], int(self.pitch / 12))
                )
            else:
                print("%sSILENCE," % (tab))
            return
        print("%sNoteSpec { .note=Note::%s%d, .vol=%d, .wav=WaveForm::%s, .fx=Effect::%s }," %
            (tab, notes[self.pitch % 12], int(self.pitch / 12), self.volume, self.wave, self.effect)
        )

    def __str__(self):
        return "note={0}{1}, vol={2}, wav={3}, effect={4}".format(
            notes[self.pitch % 12], int(self.pitch / 12), self.volume, self.wave, self.effect
        )

class Sfx:
    def __init__(self, line, data, index):
        self.data = data
        self.index = index

        self.speed = numval(line, 2, 2)
        self.loop_start = numval(line, 4, 2)
        self.loop_end = numval(line, 6, 2)
        if self.loop_end == 0:
            num_notes = 32
            self.loop_end = num_notes
            self.loop_start = self.loop_end
        else:
            num_notes = self.loop_end
        self.notes = [note_from_spec(line[8+i*5:13+i*5]) for i in range(num_notes)]

        self.custom = False
        for note in self.notes:
            if note.custom_wave():
                self.custom = True
                self.custom_inserted = False
                break

    def loops(self):
        return self.loop_end != self.loop_start

    def len(self):
        assert not self.loops()
        return len(self.notes) * self.speed

    def max_volume(self):
        return max([note.volume for note in self.notes])

    # Returns True if a specified note range contains one or more gaps
    def _part_of_arpeggio(self, note_idx):
        min_idx = int(note_idx / 4) * 4
        for note in self.notes[min_idx : min_idx + 4]:
            if note.effect == "ARPEGGIO" or note.effect == "ARPEGGIO_FAST":
                return True
        return False

    def check(self):
        if self.custom:
            warning("SFX {0} use custom instruments, which may not be rendered correctly".format(
                self.index
            ))
            self.insert_custom_notes()
            for msg in self.custom_conversion_warnings:
                warning(msg)
        for note in self.notes:
            if note.effect ==  "VIBRATO":
                if note.wave == "NOISE":
                    warning("VIBRATO effect is not supported for NOISE instrument in SFX {0}".format(
                        self.index
                    ))

    def insert_custom_notes(self):
        if self.custom_inserted:
            return

        self.custom_conversion_warnings = set()
        custom_speed = self.speed
        for note in self.notes:
            if note.custom_wave():
                custom_sfx = self.data.sfxs[note.custom_sfx_id()]
                custom_speed = min(custom_speed, custom_sfx.speed)
                if (self.speed // custom_sfx.speed) * custom_sfx.speed != self.speed:
                    self.custom_conversion_warnings.add(
                        "Custom speeds in SFX {0} not compatible with speed of SFX".format(self.index)
                    )

        custom_notes = []
        prev_note = None
        for i, note in enumerate(self.notes):
            if note.custom_wave():
                if (
                    note.effect == "DROP" or
                    prev_note is None or prev_note.wave != note.wave or prev_note.pitch != note.pitch
                ):
                    custom_idx = 0
                custom_sfx = self.data.sfxs[note.custom_sfx_id()]
                num_notes = self.speed // custom_sfx.speed
                for _ in range(num_notes):
                    custom_notes.append(adapt_custom_note(custom_sfx.notes[custom_idx], note))
                    custom_idx += 1
                    if custom_idx >= custom_sfx.loop_end:
                        if custom_sfx.loop_start < custom_sfx.loop_end:
                            custom_idx = custom_sfx.loop_start
                        else:
                            custom_idx = 0
            else:
                # Repeat the source note multiple times.
                if note.effect != "NONE":
                    self.custom_conversion_warnings.add(
                        "Replicated one or more notes in SFX {0} with an effect".format(self.index)
                    )
                custom_notes.extend([note] * (self.speed // custom_speed))
            prev_note = note

        self.notes = custom_notes
        self.loop_end *= (self.speed // custom_speed)
        self.loop_start *= (self.speed // custom_speed)
        self.speed = custom_speed

        self.custom_inserted = True

    def print(self):
        if self.custom and not self.custom_inserted:
            self.insert_custom_notes()

        print("const NoteSpec sfx%dNotes%s[%d] = {" % (self.index, postfix, len(self.notes)))
        for i, note in enumerate(self.notes):
            note.print(self._part_of_arpeggio(i))
        print("};")

        if args.comments:
            if self.loops():
                print("// SFX%d loops" % (self.index))
            else:
                print("// SFX%d len=%d" % (self.index, self.len()))

        print("const TuneSpec sfx%d%s = TuneSpec {" % (self.index, postfix))
        print("%s.noteDuration = %d, .loopStart = %d, .numNotes = %d, .notes = sfx%dNotes%s" %
            (tab, self.speed, self.loop_start, self.loop_end, self.index, postfix)
        )
        print("};")

class Pattern:
    def __init__(self, line, data, index):
        self.data = data
        self.index = index

        self.sfx_ids = [id for id in [numval(line, 3+i*2, 2) for i in range(4)] if id < 64]
        self.flags = numval(line, 0, 2)

    def is_empty(self):
        return len(self.sfx_ids) == 0

    def is_loop_start(self):
        return (self.flags & 1) != 0

    def terminates(self):
        return (self.flags & 4) != 0

    def rewinds(self):
        return (self.flags & 2) != 0

    def check(self):
        if self.is_empty():
            return

        sfxs = [self.data.sfxs[id] for id in self.sfx_ids]
        if len([sfx for sfx in sfxs if not sfx.loops()]) == 0:
            warning("All sound effects in pattern {0} loop!".format(self.index))
        elif sfxs[0].loops():
            warning("First sound effect in pattern {0} loops".format(self.index))
        elif len(sfxs) > 1:
            other_lens = [sfx.len() for sfx in sfxs[1:] if not sfx.loops()]
            if len(other_lens) > 0 and sfxs[0].len() > min(other_lens):
                warning("First sound effect in pattern {0} is not the shortest".format(self.index))

    def sfx_string(self):
        return " ".join(["SFX%d" % (id) for id in self.sfx_ids])

    def print(self):
        if self.is_empty():
            return

        print("const TuneSpec *const pattern%dTunes%s[%d] = { %s };" %
            (self.index, postfix, len(self.sfx_ids), ", ".join(
                ["&sfx%d%s" % (id, postfix) for id in self.sfx_ids]
            ))
        )

        if args.comments:
            print("// PATTERN%d: %s" % (self.index, self.sfx_string()))

        print("const PatternSpec pattern%d%s = PatternSpec {" % (self.index, postfix))
        print("%s.numTunes = %d, .tunes = pattern%dTunes%s" % (tab, len(self.sfx_ids), self.index, postfix))
        print("};")

class Song:
    def __init__(self, data, start_idx, end_idx, loop_idx = None):
        self.data = data
        self.start_idx = start_idx
        self.end_idx = end_idx
        if loop_idx is None:
            self.loop_idx = end_idx - start_idx
        else:
            self.loop_idx = loop_idx - start_idx
        self.pattern_ids = [id for id in range(start_idx, end_idx + 1)]

    def check(self):
        if self.loop_idx < 0:
            warning("Song {0} loops to earlier pattern {1}. This is unsupported".format(
                self.start_idx, self.loop_idx
            ))

    def print(self):
        print("const PatternSpec *const song%dPatterns%s[%d] = {" % (self.start_idx, postfix, len(self.pattern_ids)))
        for id in self.pattern_ids:
            print("%s&pattern%d%s," % (tab, id, postfix))
        print("};")

        print("const SongSpec song%d%s = SongSpec {" % (self.start_idx, postfix))
        print("%s.loopStart = %d, .numPatterns = %d, .patterns = song%dPatterns%s" %
            (tab, self.loop_idx, len(self.pattern_ids), self.start_idx, postfix)
        )
        print("};")

class MusicData:
    def __init__(self, filename):
        self.process_file(filename)
        self.init_songs()
        self.determine_used_sfx()

    def process_file(self, filename):
        self.sfxs = None
        self.patterns = None
        with open(filename) as fp:
            for line in fp:
                if self.sfxs is None:
                    if line.startswith("__sfx__"):
                        self.sfxs = []
                elif self.patterns is None:
                    if line.startswith("__music__"):
                        self.patterns = []
                    else:
                        self.sfxs.append(Sfx(line, self, len(self.sfxs)))
                elif len(line) >= 11:
                    self.patterns.append(Pattern(line, self, len(self.patterns)))
                else:
                    return

    def init_songs(self):
        start_idx = loop_start_idx = None

        self.songs = []
        for i, pattern in enumerate(self.patterns):
            if pattern.is_empty():
                if start_idx is not None:
                    self.songs.append(Song(self, start_idx, i - 1))
                    start_idx = None
            else:
                if start_idx is None:
                    start_idx = pattern.index
                if i == 0 or pattern.is_loop_start():
                    loop_start_idx = i
                if pattern.terminates() or (i == len(self.patterns) - 1 and not pattern.rewinds()):
                    self.songs.append(Song(self, start_idx, i))
                    start_idx = None
                elif pattern.rewinds():
                    self.songs.append(Song(self, start_idx, i, loop_start_idx))
                    start_idx = None

    def determine_used_sfx(self):
        # Determine the SFXs that are used in one or more songs
        self.used_sfx_ids = set()
        for song in self.songs:
            for pattern_id in song.pattern_ids:
                pattern = self.patterns[pattern_id]
                for sfx_id in pattern.sfx_ids:
                    self.used_sfx_ids.add(sfx_id)

    def check(self):
        for sfx in self.sfxs:
            sfx.check()
        for pattern in self.patterns:
            pattern.check()
        for song in self.songs:
            song.check()

        max_volume = max([sfx.max_volume() for sfx in [self.sfxs[sfx_id] for sfx_id in self.used_sfx_ids]])
        if max_volume < 7:
            warning("The available volume range is not fully used: max = {0} < 7".format(max_volume))

        d = {}
        for i, pattern in enumerate(self.patterns):
            pattern_spec = pattern.sfx_string()
            if pattern_spec in d:
                warning("Pattern {0} is a duplicate and can be replaced by {1}".format(i, d[pattern_spec]))
            else:
                d[pattern_spec] = i

    def print(self):
        print("// This file is automatically generated using import-from-p8.py")
        print()
        for sfx_id in self.used_sfx_ids:
            sfx = self.sfxs[sfx_id]
            sfx.print()
        print()
        for pattern in self.patterns:
            pattern.print()
        print()
        for song in self.songs:
            song.print()

music_data = MusicData(args.filename)
music_data.check()
music_data.print()