def parse(source, key):

    print "source: {} \nkey: {}".format(source, key)
    parseMessage = source.split(" ")
    for x in range(0, len(parseMessage)):
    	print x
        if key in parseMessage[x]:
            return parseMessage[x+1].strip()
    print "Parse error on {}: key {} not found".format(source, key)
    return ""

def main():
	test = "THE IS A test FILE TEST file"
	print test
	print parse(test, "IS")

if __name__ == '__main__':
    try:
        main()
    except KeyboardInterrupt:
        print "Keyboard Interrupt"
        sys.exit(1)