import vk
import os
from flask import Flask, json, request, send_from_directory, redirect
from flask_restful import Api,Resource
from flask_cors import CORS


vk_session = vk.Session(access_token=os.environ['VK_TOKEN'])
vk_api = vk.API(vk_session)

app = Flask(__name__, static_url_path='/')
flask_api = Api(app)
CORS(app)


@app.route('/')
def no_index():
    return redirect("/index.html", code=302)


@app.route('/<path:path>')
def index(path):
    return send_from_directory('/static/', path)


class Photos(Resource):

    def __init__(self):
        self.photos = []

    @staticmethod
    def parse_params():
        owner_id = request.args.get('o')
        album_id = request.args.get('a')
        photo_size = request.args.get('s')
        return owner_id, album_id, photo_size

    def vk_request(self, owner_id, album_id, photo_size):
        vk_response = vk_api.photos.get(owner_id=owner_id, album_id=album_id, photo_sizes=1, count=1000, v="5.101")
        for photo in vk_response['items']:
            self.photos.append([size['url'] for size in photo['sizes'] if size['type'] == photo_size])
        return self.photos

    def get(self):
        owner_id, album_id, photo_size = self.parse_params()
        self.vk_request(owner_id, album_id, photo_size)
        response = app.response_class(
                response=json.dumps(self.photos),
                status=200,
                mimetype='application/json'
            )
        return response


flask_api.add_resource(Photos, '/photos')


if __name__ == '__main__':
    app.run(host='0.0.0.0', port='80')
