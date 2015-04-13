import java.util.ArrayList;
import java.util.LinkedList;
import java.util.Scanner;
import java.util.Collections;


 public class NFA {
	int startState;
	int finalState;
	ArrayList<transition> Transitions;
	
	public NFA(int ss, int es){
		startState = ss;
		finalState = es;
		Transitions = new ArrayList<transition>();
	}
	
	public NFA(int ss, int es, transition trans){
		startState = ss;
		finalState = es;
		Transitions = new ArrayList<transition>();
		Transitions.add(trans);
	}
		
	public NFA(int ss, int es, ArrayList<transition> trans){
		startState = ss;
		finalState = es;
		Transitions = new ArrayList<transition>();
		Transitions.addAll(trans);
	}
	
	public static NFA or(int n, NFA nfa1, NFA nfa2){ // + of two NFAs					
		nfa1.Transitions.add(new transition(n, nfa1.startState, 'E'));
		nfa1.Transitions.add(new transition(n, nfa2.startState, 'E'));		
		nfa1.Transitions.add(new transition(nfa1.finalState, n+1, 'E'));		
		nfa1.Transitions.add(new transition(nfa2.finalState, n+1, 'E'));
		nfa1.Transitions.addAll(nfa2.Transitions);
		Collections.sort(nfa1.Transitions);
		nfa1.startState = n;
		nfa1.finalState = n+1;
		return nfa1;
	}
	
	private static NFA and(NFA nfa1, NFA nfa2) { // & of two NFAs
		nfa1.Transitions.add(new transition(nfa1.finalState, nfa2.startState, 'E'));
		nfa1.Transitions.addAll(nfa2.Transitions);
		nfa1.finalState = nfa2.finalState;
		Collections.sort(nfa1.Transitions);
		return nfa1;
	}
	
	private static NFA kleene(NFA nfa) { // * of an NFAs
		nfa.Transitions.add(new transition(nfa.finalState, nfa.startState, 'E'));
		nfa.finalState = nfa.startState;
		Collections.sort(nfa.Transitions);
		return nfa;
	}
	
	public String toString(){			
		return ("\nStart state: q" + this.startState + "\nFinal state: q" + this.finalState + "\n\nTransitions: " + this.Transitions.toString());
	}
	
	public static void main(String arg[]){
		LinkedList<NFA> stack = new LinkedList<NFA>();
		Scanner sc = new Scanner(System.in);
		int j = 1;
		while(sc.hasNextLine() == true){
			String regex = sc.nextLine();		
			System.out.print("\n\n\nRegular Expression " + j++ + ": " + regex + "\n");
			NFA nfa1 = null, nfa2 = null;
			int n = 0; //state number
			for(int i = 0; i < regex.length(); i++){
				switch(regex.charAt(i)){
				case 0:
					break;
				case 'a':
				case 'b':
				case 'c':
				case 'd':
				case 'e':
					stack.push(new NFA(n, n+1, new transition(n, n+1, regex.charAt(i))));	
					n += 2;
					break;
				case 'E':
					stack.push(new NFA(n, n));	
					n++;	
					break;
				case '+':
					if(stack.size() < 2){
						//System.out.print("Error: Incorrect number of operands.\n");
						continue;
					}
					nfa2 = stack.pop();
					nfa1 = stack.pop();
					stack.push(or(n, nfa1, nfa2));
					n+=2;
					break;
				case '&':
					if(stack.size() < 2){
						//System.out.print("Error: Incorrect number of operands.\n");
						continue;
					}
					nfa2 = stack.pop();
					nfa1 = stack.pop();
					stack.push(and(nfa1, nfa2));
					break;
				case '*':
					if(stack.size() == 0){
						//System.out.print("Error: Incorrect number of operands.\n");
						continue;
					}
					nfa1 = stack.pop();
					stack.push(kleene(nfa1));
					break;
				default:
					System.out.print("Error: Character " + regex.charAt(i) + " is not recognized. Skipping to next character.\n");
				}		
			}
			if (stack.size() != 1){
				System.out.print("Error: Incorrect format. Aborting.\n");
				stack.clear();
				continue;
			}
			nfa1 = stack.pop();
			if(nfa1 != null)
				System.out.println(nfa1.toString());
		}
		sc.close();
	}
}

