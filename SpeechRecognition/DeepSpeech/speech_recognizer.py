#!/usr/bin/env python
# -*- coding: utf-8 -*-
from __future__ import absolute_import, division, print_function

import numpy as np
import sys
import wave
import io

from deepspeech import Model
from timeit import default_timer as timer


class SpeechRecognizer:
    def __init__(self, model):
        print('Loading model from file {}'.format(model), file=sys.stderr)
        model_load_start = timer()
        self.ds = Model(model)
        model_load_end = timer() - model_load_start
        print('Loaded model in {:.3}s.'.format(model_load_end), file=sys.stderr)
        self.desired_sample_rate = self.ds.sampleRate()

    def recognize(self, audio):
        result = {'error': '', 'text': '', 'inference_length': 0, 'audio_length': 0}
        fin = wave.open(io.BytesIO(audio), 'rb')
        fs_orig = fin.getframerate()
        if fs_orig != self.desired_sample_rate:
            result['error'] = 'Error: original sample rate ({}) is different than {}hz'.format(fs_orig, self.desired_sample_rate)
            return result
        else:
            audio = np.frombuffer(fin.readframes(fin.getnframes()), np.int16)

        audio_length = fin.getnframes() * (1 / fs_orig)
        fin.close()

        inference_start = timer()
        # stream = self.ds.createStream()
        # stream.
        text = self.ds.stt(audio)
        result['text'] = text
        inference_end = timer() - inference_start
        result['inference_length'] = inference_end
        result['audio_length'] = audio_length
        return result


# a = SpeechRecognizer('deepspeech-0.7.0-models.pbmm')
# print(a.recognize('sample5.wav'))
# a.recognize('sample1.wav')
