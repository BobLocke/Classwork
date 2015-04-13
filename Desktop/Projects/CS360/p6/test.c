

boolean TestAndSet(boolean *target) {
			boolean rv = target;
			target = true;
			return rv;
		}

int main(void){
	boolean target = false;
	boolean hum = TestandSet(target);

	printf("%s", hum);

	return 0;
}