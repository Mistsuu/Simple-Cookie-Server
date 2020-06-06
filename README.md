# cookie_server
Cookie Server for Network Programming Final...

# Format for GET/SET request

<b>SET</b> ```SET <key0>=<value0>; <key1>=<value1>; <key2>=<value2>; (...)  # <- Ends with ;``` <br>
<b>GET</b> ```GET <key>;                                                    # <- Also ends with ; (JUST 1 KEY GET AT A TIME!!)```

# Running... But how?
1. You connect me.\
2. You send me request.\
3. I do your request.\
	
	SET: I do your thing, nothing sends back to you, then close connection.\
	GET: I help you get your value, sends you the value, then close connection.\

4. Any question?\

