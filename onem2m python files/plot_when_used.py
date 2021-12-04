import matplotlib.pyplot as plt
import json
import requests
from datetime import datetime

ae = "Smart-water-despenser"
cnt = ["water-level", "when-used", "when-refilled"]
value_axis = []
time_axis = []


headers = {
    'X-M2M-Origin' : 'admin:admin',
    'Content-type' : 'application/json'
}

response = requests.get(f'http://127.0.0.1:8080/~/in-cse/in-name/{ae}/{cnt[1]}/?rcn=4', headers=headers)
data = json.loads(response.text)

for i in data['m2m:cnt']['m2m:cin']:
    time_axis.append(datetime.strptime(i['ct'], '%Y%m%dT%H%M%S'))
    value_axis.append(int(i['con']))
    

plt.plot(time_axis, value_axis)
plt.ylabel('When used')
plt.xlabel('Time')
plt.show()