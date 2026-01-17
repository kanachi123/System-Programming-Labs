from flask import Flask

app = Flask(__name__)

@app.route('/')
def home():
    return "Hello from the Flask server!"

if __name__ == "main":
    app.run(host="127.0.0.1",port=5000)