from speech_recognizer import SpeechRecognizer
import socket
import asyncore
import numpy as np
import webrtcvad

recognizer = SpeechRecognizer('deepspeech-0.7.0-models.pbmm')
ds_stream = recognizer.ds.createStream()

vad = webrtcvad.Vad(3)

rate = 16000
duration = 30
end_silence_timeout_ms = 900
max_silence_count = int(end_silence_timeout_ms / duration)
empty_audio = b'\x00\x00' * 10000

silence_count = max_silence_count
is_ds_stream = False
audio_buffer = b''


class EchoHandler(asyncore.dispatcher_with_send):
    def handle_read(self):
        data = self.recv(100000)
        self.parse_audio(data)

    def parse_audio(self, input_data):
        global recognizer, ds_stream, silence_count, max_silence_count, audio_buffer, empty_audio, is_ds_stream
        global duration, rate

        audio_buffer += input_data
        size = int(rate * duration / 1000)
        speech_count = int(len(audio_buffer) / (2 * size))

        for i in range(speech_count):
            data = audio_buffer[i * size * 2:(i + 1) * size * 2]
            is_speech = vad.is_speech(data, rate)

            if is_speech:
                silence_count = 0
            else:
                silence_count += 1

            if silence_count < max_silence_count:
                if not is_ds_stream:
                    ds_stream = recognizer.ds.createStream()
                    data = empty_audio + data
                    is_ds_stream = True
                print('feed', silence_count)
                ds_stream.feedAudioContent(np.frombuffer(data, dtype=np.int16))
            elif is_ds_stream:
                text = ds_stream.finishStream()
                print('recognized: ', text)
                self.send(str.encode(text))
                is_ds_stream = False

        audio_buffer = audio_buffer[speech_count * 2 * size:]


class EchoServer(asyncore.dispatcher):
    def __init__(self):
        asyncore.dispatcher.__init__(self)
        self.create_socket(socket.AF_INET, socket.SOCK_STREAM)
        self.bind(('127.0.0.1', 5001))
        self.listen(1)
        print('wait connection')

    def handle_accept(self):
        sock, addr = self.accept()
        print('connection from ', addr)
        handler = EchoHandler(sock)


def start_echo_server():
    server = EchoServer()
    asyncore.loop()


start_echo_server()
