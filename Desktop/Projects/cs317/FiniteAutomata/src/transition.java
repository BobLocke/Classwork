
public class transition implements Comparable<transition>{
	
	int beginState;
	int endState;
	char symbol;
	
	transition(int ss, int es, char x){
		beginState = ss;
		endState = es;
		symbol = x;
	}
	
	public int compareTo(transition t2){
		if(this.beginState > t2.beginState) return 1;
		else if(this.beginState == t2.beginState){
			if(this.endState >= t2.endState)
				return 1;
			else return -1;
		}
		return -1;
	}
	
	public String toString(){
		return "(q" + this.beginState + ", q" + this.endState + ", " + symbol +")";
	}
}