import requests

with requests.Session() as s:
    s.post("http://ugster05.student.cs.uwaterloo.ca/jeadie/post.php", data={"form": "login", "username": "alice", "password": "password458"})
    s.get("http://ugster05.student.cs.uwaterloo.ca/jeadie/vote.php?id=6&vote=1")

