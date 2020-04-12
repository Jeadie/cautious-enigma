import requests
import base64

for i in range(100, 101):
    c = base64.b64encode(b'sonicmega-67').decode()
    print(c)
    code = "c29uaWNtZWdhLTY3"
    with requests.Session() as s:
        print(i)
        s.get(f"http://ugster05.student.cs.uwaterloo.ca/jeadie/confirm.php?code={str(code)}")
        s.post("http://ugster05.student.cs.uwaterloo.ca/jeadie/post.php", data={"type": "1", "form": "content", "title":"Easy Password", "content": str(i)})
        s.get("http://ugster05.student.cs.uwaterloo.ca/jeadie/logout.php")
