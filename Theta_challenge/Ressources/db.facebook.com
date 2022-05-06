;
; BIND data file for local loopback interface
;
$TTL    604800
@       IN      SOA     facebook.com. root.facebook.com. (
                              2         ; Serial
                         604800         ; Refresh
                          86400         ; Retry
                        2419200         ; Expire
                         604800 )       ; Negative Cache TTL
;
@       IN      NS      ns.facebook.com.
@       IN      A       10.0.2.15
@       IN      AAAA    ::1
NS      IN      A       10.0.2.15
www     IN      A       10.0.2.15
;kk	IN	A	10.0.2.15
