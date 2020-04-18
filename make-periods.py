import math

num_notes = 12
x = math.e ** (math.log(2) / num_notes)

sample_rate = 11025
freq_a4 = 440

# calculate period for c0
p = (sample_rate / freq_a4) * 2**5 * x**-3

# Scale maximally so that it still fits in 15 bits
p *= 2**5
for _ in range(num_notes):
    print("%d, " % (int(p + 0.5)), end='')
    p /= x
