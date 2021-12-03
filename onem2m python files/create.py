from onem2m import *
uri_cse = "http://127.0.0.1:8080/~/in-cse/in-name"

ae = "Smart-water-despenser"

cnt = "water-level";
cnt1 = "when-used";
cnt2 = "when-refilled"

create_ae(uri_cse, ae)

uri_ae = uri_cse + "/" + ae
create_cnt(uri_ae, cnt)
create_cnt(uri_ae, cnt1)
create_cnt(uri_ae, cnt2)