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

response = requests.get(f'http://127.0.0.1:8080/~/in-cse/in-name/{ae}/{cnt[2]}/?rcn=4', headers=headers)
data = json.loads(response.text)

for i in data['m2m:cnt']['m2m:cin']:
    datetime_obj = datetime.strptime(i['ct'], '%Y%m%dT%H%M%S')
    t = datetime_obj.time()
    minute = t.minute
    minute*=60
    hours = t.hour
    hours*=3600
    x=(t.second+minute+hours)
    time_axis.append(x)
    value_axis.append(int(i['con']))
    
plt.plot(time_axis, value_axis,'-ok',color='orange')
plt.ylabel('When refilled')
plt.xlabel('Time')
plt.show()
