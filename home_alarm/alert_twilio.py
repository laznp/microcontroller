from flask import Flask, jsonify, request
from twilio.rest import Client
app = Flask(__name__)
app.secret_key = "SecretKey!"

@app.route("/")
def index():
	account_sid = 'twilio_account_id'
	auth_token = 'twilio_auth_token'
	client = Client(account_sid, auth_token)

	if "X-Device-ID" in request.headers and request.headers["X-Device-ID"] == "ESP8266":
		call = client.calls.create(
			url='http://demo.twilio.com/docs/voice.xml',
			to='target_phone_number',
			from_='twilio_phone_number'
		)
		return jsonify({"sid":call.sid})
	else:
		return jsonify({"error":"GOBLOK!"})

if __name__ == '__main__':
	app.run(debug=True)