from argparse import ArgumentParser

tab = "    "
notes = ["C", "Cs", "D", "Ds", "E", "F", "Fs", "G", "Gs", "A", "As", "B"]
waves = ["TRIANGLE", "TILTED_SAW", "SAW", "SQUARE", "PULSE", "ORGAN", "NOISE", "PHASER"]
effects = ["NONE", "SLIDE", "VIBRATO", "DROP", "FADE_IN", "FADE_OUT", "ARPEGGIO", "ARPEGGIO_FAST"]

parser = ArgumentParser()
parser.add_argument(
    "filename", help="PICO-8 source file"
)
args = parser.parse_args()

def numval(line, pos, num_chars):
    return int(line[pos:pos+num_chars], 16)

def warning(msg):
    print("// WARNING: %s" % (msg))

class Note:
    def __init__(self, spec):
        print("notespec =", spec)
        self.volume = numval(spec, 3, 1)
        noderank = numval(spec, 0, 2)
        self.note = notes[noderank % 12]
        self.octave = 2 + int(noderank / 12)
        self.wave = waves[numval(spec, 2, 1)]
        self.effect = effects[numval(spec, 4, 1)]

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
        print("init Sfx", index)
        self.data = data
        self.index = index

        self.speed = numval(line, 2, 2)
        self.loop_start = numval(line, 4, 2)
        self.loop_end = numval(line, 6, 2)
        self.num_notes = 32
        if self.loop_end == 0:
            self.loop_end = self.num_notes
            self.loop_start = self.loop_end
        self.notes = [Note(line[8+i*5:13+i*5]) for i in range(self.num_notes)]

    def loops(self):
        return self.loop_end != self.loop_start

    def len(self):
        assert not self.loops()
        return self.num_notes * self.speed

    def check(self):
        pass

    def print(self):
        print("const NoteSpec sfx%dNotes[%d] = {" % (self.index, self.num_notes))
        for note in self.notes:
            note.print()
        print("};")

        print("const TuneSpec sfx%d = TuneSpec {" % (self.index))
        print("%s.noteDuration = %d, .loopStart = %d, .numNotes = %d, .notes = sfx%dNotes" %
            (tab, self.speed, self.loop_start, self.loop_end, self.index)
        )
        print("};")

class Pattern:
    def __init__(self, line, data, index):
        print("init Pattern", index)
        self.data = data
        self.index = index

        self.sfx_ids = [id for id in [numval(line, 3+i*2, 2) for i in range(4)] if id < 64]

    def check(self):
        if len(self.sfx_ids)==0:
            return

        sfxs = [self.data.sfxs[id] for id in self.sfx_ids]
        if len([sfx for sfx in sfxs if not sfx.loops()]) == 0:
            warning("All sound effects in pattern %d loop!".format(self.index))
        elif sfxs[0].loops():
            warning("First sound effect in pattern %d loops".format(self.index))
        elif len(sfxs) > 1 and sfxs[0].len() > min([sfx.len() for sfx in sfxs[1:] if not sfx.loops()]):
            warning(("First sound effect in pattern %d is not the shortest".format(self.index)))

    def print(self):
        if len(self.sfx_ids)==0:
            return

        print("const TuneSpec* pattern%dTunes[%d] = { %s };" %
            (self.index, len(self.sfx_ids), ", ".join(["&sfx%d" % (id) for id in self.sfx_ids]))
        )

        print("const PatternSpec pattern%d = PatternSpec {" % (self.index))
        print("%s.numTunes = %d, .tunes = pattern%dTunes" % (tab, len(self.sfx_ids), self.index))
        print("};")

class MusicData:
    def __init__(self, filename):
        self.process_file(filename)

    def process_file(self, filename):
        self.sfxs = None
        self.patterns = None
        with open(filename) as fp:
            for line in fp:
                print(line)
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

    def check(self):
        for sfx in self.sfxs:
            sfx.check()
        for pattern in self.patterns:
            pattern.check()

    def print(self):
        for sfx in self.sfxs:
            sfx.print()
        print()
        for pattern in self.patterns:
            pattern.print()

music_data = MusicData(args.filename)
music_data.check()
music_data.print()