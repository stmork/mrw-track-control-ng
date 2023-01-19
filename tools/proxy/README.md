# The MRW-Proxy tool
The MRW-Proxy tool simply connects a real CAN bus to a virtual CAN bus over TCP. In fact it uses the Qt feature of socketcan and virtualcan plugins. Each frame received from on bus is sent to the other and vice versa.

See: https://doc.qt.io/qt-6/qtcanbus-backends.html
