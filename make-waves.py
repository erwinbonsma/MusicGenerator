
import random

random.seed(1982)

def lerp(x, v1, v2):
    return (1 - x) * v1 + x * v2

class pelgrin:

    def __init__(self, num_samples):
        self.n = num_samples
        self.rnd_vals = [2 * (random.random() - 0.5) for x in range(self.n)]
        self.rnd_vals[0] = 0 # Don't distort the boundary values
        self.rnd_vals.append(self.rnd_vals[0]) # Avoid index modulus calculation

    def _add_noise(self, samples, step, mul, start_index):
        for i in range(self.n):
            i0 = i - (i % step)
            if (self.n - i) <= step:
                if step > 1:
                    v = (i - i0) / (step - 1)
                else:
                    v = 1
            else:
                v = (i - i0) / step
            delta = lerp(v, self.rnd_vals[i0], self.rnd_vals[i0 + step])
            samples[i + start_index] += mul * delta

    def add_noise(self, samples, mul = 1, start_index = 0):
        step = self.n
        while 1:
            step >>= 1
            mul *= 0.5
            if step == 0:
                break
            self._add_noise(samples, step, mul, start_index)

def output_samples(samples):
    n = 0
    for v in samples:
        print("%4d, " % (v), end='')
        n += 1
        if n % 16 == 0:
            print()

def scale(samples):
    mn = min(samples)
    mx = max(samples)
    return [int(lerp((x - mn) / (mx - mn), -128, 127)) for x in samples]

def make_noise_wave():
    n = 256

    w = []
    w.extend([x for x in range(-128, 127)])
    w.extend([x for x in range(127, -128, -1)])
    w.append(-128) # Extra sample so that pelgrin noise can be added to second half

    mul = 128
    pelgrin(n).add_noise(w, mul)
    pelgrin(n).add_noise(w, mul, n - 1)

    w = scale(w)

    output_samples(w[:n-1])
    print()
    output_samples(w[n-1:])

    return w

make_noise_wave()
