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

class Note:
    def __init__(self, spec):
        self.volume = numval(spec, 3, 1)
        noderank = numval(spec, 0, 2)
        self.note = notes[noderank % 12]
        self.octave = 2 + int(noderank / 12)
        wave_num = numval(spec, 2, 1)
        if wave_num < 8:
            self.wave = waves[wave_num]
        else:
            self.wave = "CUSTOM{0}".format(wave_num - 8)
        self.effect = effects[numval(spec, 4, 1)]

    def custom_wave(self):
        return self.wave.startswith("CUSTOM")

    def print(self):
        if self.volume == 0:
            print("%sSILENCE," % (tab))
            return
        # Note: Adding one to volume to handle range mismatch: PICO-8 range is [0, 7], Gamebuino
        # range is [0, 8]. This means that volume 1 will never be used. This could be re-introduced
        # via manual post-processing.
        print("%sNoteSpec { .note=Note::%s, .oct=%d, .vol=%d, .wav=WaveForm::%s, .fx=Effect::%s }," %
            (tab, self.note, self.octave, self.volume + 1, self.wave, self.effect)
        )

class Sfx:
    def __init__(self, line, data, index):
        self.data = data
        self.index = index

        self.speed = numval(line, 2, 2)
        self.loop_start = numval(line, 4, 2)
        self.loop_end = numval(line, 6, 2)
        if self.loop_end == 0:
            self.num_notes = 32
            self.loop_end = self.num_notes
            self.loop_start = self.loop_end
        else:
            self.num_notes = self.loop_end
        self.notes = [Note(line[8+i*5:13+i*5]) for i in range(self.num_notes)]

    def loops(self):
        return self.loop_end != self.loop_start

    def len(self):
        assert not self.loops()
        return self.num_notes * self.speed

    def max_volume(self):
        return max([note.volume for note in self.notes])

    def check(self):
        for note in self.notes:
            if note.custom_wave():
                warning("One or more notes in SFX {0} use a custom instrument, which needs replacing".format(
                    self.index
                ))
                break

    def print(self):
        print("const NoteSpec sfx%dNotes%s[%d] = {" % (self.index, postfix, self.num_notes))
        for note in self.notes:
            note.print()
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

        print("const TuneSpec* pattern%dTunes%s[%d] = { %s };" %
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
        print("const PatternSpec* song%dPatterns%s[%d] = {" % (self.start_idx, postfix, len(self.pattern_ids)))
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