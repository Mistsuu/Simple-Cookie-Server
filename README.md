# cookie_server
Cookie Server for Network Programming Final...

# Format for GET/SET request

<b>SET</b> ```SET <key0>=<value0>; <key1>=<value1>; <key2>=<value2>; (...)  # <- Ends with ;``` <br>
<b>GET</b> ```GET <key>;                                                    # <- Also ends with ; (JUST 1 KEY GET AT A TIME!!)```

# Running... But how?
<b>1</b>. You connect me.
<b>2</b>. You send me request.
<b>3</b>. I do your request.
	
	<b>SET</b>: I do your thing, nothing sends back to you, then close connection.
	<b>GET</b>: I help you get your value, sends you the value, then close connection.

<b>4</b>. Any question?

