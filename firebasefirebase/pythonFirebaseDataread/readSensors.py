# Done from youtube channel
# All About Python. vid link is "https://www.youtube.com/watch?v=BnrkTpgH5Vc"


import firebase_admin
from firebase_admin import db,credentials

# credentials Setup here
cred=credentials.Certificate("credentials.json")
firebase_admin.initialize_app(cred,{"databaseURL":"https://esp8266-data-transfer-default-rtdb.firebaseio.com/"})

# just creating a reference shortcut
ref = db.reference("/")

ref.get()

# db.reference() is used to get the data returns string of the getting info.
print(db.reference("/test").get())
ref.get();
