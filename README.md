# cookie_server
A simple SET-GET server...

## Format for GET/SET request
<b>SET</b> ```SET <key0>=<value0>; <key1>=<value1>; <key2>=<value2>; (...)  # <- Ends with ;``` <br>
<b>GET</b> ```GET <key>;                                                    # <- Also ends with ; (JUST 1 KEY GET AT A TIME!!)```

## How the server works
<b>1</b>. You connect me. <br>
<b>2</b>. You send me a SET or GET request. <br>
<b>3</b>. I do your request. <br>
	
	SET: You want to set a value for a key, I'll do that for you, then I close the connection.
	GET: You tell me to help you get your value from a key, I send you the value, then close connection.

