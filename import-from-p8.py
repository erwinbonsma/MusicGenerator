from argparse import ArgumentParser

tab = "    "
notes = ["A", "As", "B", "C", "Cs", "D", "Ds", "E", "F", "Fs", "G", "Gs"]
waves = ["TRIANGLE", "TILTED_SAW", "SAW", "SQUARE", "PULSE", "ORGAN", "NOISE", "PHASER"]
effects = ["NONE", "SLIDE", "VIBRATO", "DROP", "FADE_IN", "FADE_OUT", "ARPEGGIO", "ARPEGGIO_FAST"]

parser = ArgumentParser()
parser.add_argument(
    "filename", help="PICO-8 source file"
)
args = parser.parse_args()

def numval(line, pos, len):
    return int(line[pos:pos+len], 16)

def numnotes(line):
    for i in range(32, 0, -1):
        if line[8+(i-1)*5+3] != "0":
            return i
    return 0

def output_note(notespec):
    #print("notespec =", notespec)
    volume = numval(notespec, 3, 1)
    if volume == 0:
        print("%s%sSILENCE," % (tab, tab))
        return
    noderank = numval(notespec, 0, 2)
    note = notes[(noderank + 3) % 12]
    octave = 2 + int((noderank + 3) / 12)
    wave = waves[numval(notespec, 2, 1)]
    effect = effects[numval(notespec, 4, 1)]
    print("%s%sNoteSpec { .note=Note::%s, .oct=%d, .vol=%d, .wav=WaveForm::%s, .fx=Effect::%s }," %
        (tab, tab, note, octave, volume, wave, effect)
    )


def output_sfx(line, sfx_id):
    speed = numval(line, 2, 2)
    loop_start = numval(line, 4, 2)
    loop_end = numval(line, 6, 2)
    num_notes = numnotes(line)
    if loop_end == 0:
        loop_end = num_notes
        loop_start = loop_end
    print("const TuneSpec sfx%d = TuneSpec {" % (sfx_id))
    print("%s.noteDuration = %d, .loopStart = %d, .loopEnd = %d," %
        (tab, speed, loop_start, loop_end)
    )
    print("%s.notes = new NoteSpec[%d] {" % (tab, num_notes))
    for i in range(num_notes):
        output_note(line[8+i*5:13+i*5])
    print("%s}" % (tab))
    print("};")

def process_file(filename):
    with open(filename) as fp:
        phase = 0
        num_sfx = 0
        for line in fp:
            if phase==0:
                if line.startswith("__sfx__"):
                    phase = 1
            elif phase==1:
                if not line.startswith("__music__"):
                    output_sfx(line, num_sfx)
                    num_sfx += 1
                else:
                    phase = 2

process_file(args.filename)