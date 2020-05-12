import flask
from flask import request, jsonify
from speech_recognizer import SpeechRecognizer

recognizer = SpeechRecognizer("deepspeech-0.7.0-models.pbmm")
app = flask.Flask(__name__)
app.config["DEBUG"] = True


@app.route('/recognize', methods=['POST'])
def recognize():
    global recognizer
    data = request.data
    result = recognizer.recognize(data)
    print(result)
    return jsonify(result)


app.run()
