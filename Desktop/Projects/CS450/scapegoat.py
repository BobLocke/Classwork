import math

rebuildArray = []
printTreeStrings = [""]

class Node:

	def __init__(self, key, leftchild, rightchild, parent):

		self.key = key
		self.leftchild = leftchild
		self.rightchild = rightchild
		self.parent = parent

class Tree:

	def __init__(self, alpha, key):
		self.root = Node(key, None, None, None)
		self.size = 1
		self.maxsize = 1
		self.alpha = alpha

	def insert(self, key, node = 0):
		if node == 0:
			node = self.root
		if key < node.key:
			if node.leftchild:
				self.insert(key, node.leftchild)
			else:
				node.leftchild = Node(key, None, None, node)
				self.size+=1
				self.maxsize = max(self.size, self.maxsize)
				depth = self.depthN(node.leftchild)
				#print("inserting {}...".format(key))
				#print("{} < {}?".format(depth, math.log(self.size, 1/self.alpha)))
				if depth > (math.log(self.size, 1/self.alpha)):
					self.rebalance(node.leftchild)				
		else:
			if node.rightchild:
				return self.insert(key, node.rightchild)
			else:
				node.rightchild = Node(key, None, None, node)
				self.size+=1
				self.maxsize = max(self.size, self.maxsize)
				depth = self.depthN(node.rightchild)
				#print("inserting {}...".format(key))
				#print("{} < {}?".format(depth, math.log(self.size, 1/self.alpha)))
				if depth > (math.floor(math.log(self.size, 1/self.alpha))):
					self.rebalance(node.rightchild)
		return

	def rebalance(self, node = 0):
		if node == 0:
			node = self.root
		print("rebalance key: {}".format(node.key))
		scapegoat = node
		while(node.parent):
			node = node.parent
			if self.sizeT(node.leftchild) > self.alpha * self.sizeT(node) or self.sizeT(node.rightchild) > self.alpha * self.sizeT(node):
				scapegoat = node
		print("Scapegoat key: {}".format(scapegoat.key))
		print("children of scapegoat: {}, {}".format(scapegoat.leftchild, scapegoat.rightchild.key))
		self.buildArray(scapegoat)
		print(rebuildArray)
		print("midpoint is {}".format(int(math.floor(float(len(rebuildArray))/2))))
		scapegoat.key = rebuildArray[int(math.floor(float(len(rebuildArray))/2))]
		print(0, int (math.floor(len(rebuildArray)/2))-1)
		print(int (math.floor(float(len(rebuildArray))/2)+1), len(rebuildArray))
		scapegoat.leftchild = self.rebuild(0, int (math.floor(float(len(rebuildArray))/2))-1)
		scapegoat.rightchild = self.rebuild(int (math.floor(float(len(rebuildArray))/2)+1), len(rebuildArray))
		self.reParent(scapegoat)
		for x in range(0, len(rebuildArray)):
			rebuildArray.pop()
		return	

	def buildArray(self, node):
		rebuildArray.append(node.key)
		if node.leftchild:
			self.buildArray(node.leftchild)
		if node.rightchild:
			self.buildArray(node.rightchild)
		rebuildArray.sort()
		return

	def reParent(self, node):
		if node.leftchild:
			node.leftchild.parent = node
			self.reParent(node.leftchild)
		if node.rightchild:
			node.rightchild.parent = node
			self.reParent(node.rightchild)
		return


	def rebuild(self, start, end):
		if end >= len(rebuildArray):
			end = len(rebuildArray) - 1
		print(start, end)
		if start == end:
			print("midpoint is {}, value is {}".format(start, rebuildArray[start]))
			return Node(rebuildArray[start], None, None, None)
		if start > end:
			print("done")
			return None
		midpoint = int (math.floor(start + (end - start)/2))
		print("midpoint is {}, value is {}".format(midpoint, rebuildArray[midpoint]))
		temp = Node(rebuildArray[midpoint], None, None, None)
		temp.leftchild = self.rebuild(start, midpoint-1)
		if end - start == 1:
			pass
		else:
			temp.rightchild = self.rebuild(midpoint+1, end)
		return temp

	def sizeT(self, node):
		if node == None:
			return 0
		else:
		 	return self.sizeT(node.leftchild) + self.sizeT(node.rightchild) + 1	

	def depthN(self, node):
		depth = 0
		while node.parent:
			node = node.parent
			depth += 1
		return depth

	def delete(self, key):
		self.size-=1
		delNode = self.search(key)
		if not delNode.rightchild and not delNode.leftchild:
			if delNode == delNode.parent.leftchild:
				delNode.parent.leftchild = None
				if self.size <= self.alpha * self.maxsize:
					self.rebalance()
				return
			else:
				delNode.parent.rightchild = None
				if self.size <= self.alpha * self.maxsize:
					self.rebalance()
				return
		if delNode.rightchild:
			successor = delNode.rightchild
			while successor.leftchild:
				successor = successor.leftchild
			if delNode.rightchild == successor:
				#print(successor.key)
				if successor.rightchild:
					delNode.rightchild = successor.rightchild
				else: 
					delNode.rightchild = None
				delNode.key = successor.key
				if self.size <= self.alpha * self.maxsize:
					self.rebalance()
				return
			if successor.rightchild:
				successor.parent.leftchild = successor.rightchild
				successor.rightchild.parent = sucessor.parent
				if self.size <= self.alpha * self.maxsize:
					self.rebalance()
				return
			else:
				successor.parent.leftchild = None
				delNode.key = successor.key
				if self.size <= self.alpha * self.maxsize:
					self.rebalance()
				return
		else:	
			if delNode == self.root:
				self.root = delNode.leftchild 
				self.root.parent = None
				if self.size <= self.alpha * self.maxsize:
					self.rebalance()
				return
			if delNode == delNode.parent.leftchild:
				delNode.parent.leftchild = delNode.leftchild
				if self.size <= self.alpha * self.maxsize:
					self.rebalance()
				return
			else:
				delNode.parent.rightchild = delNode.leftchild
				if self.size <= self.alpha * self.maxsize:
					self.rebalance()
				return

	def search(self, key, node = 0):
		if node == 0:
			node = self.root
		if key == node.key:
			return node
		elif key > node.key:
			return self.search(key, node.rightchild)
		elif key < node.key:
			return self.search(key, node.leftchild)


	"""

	def printTree(self, node = 0):
		if node == 0:
			node = self.root
		print("-({})".format(node.key))
		if node.leftchild:
			self.pTree(1, node.leftchild, 'L')
		if node.rightchild:
			self.pTree(1, node.rightchild, 'R')

	def pTree(self, dashes, node, LR):
		pSt = "-"
		for x in range(0, dashes):
			pSt = pSt + '-'
		pSt += "({}{})".format(LR, node.key)
		print(pSt)
		if node.leftchild:
			self.pTree(dashes+1, node.leftchild, 'L')
		if node.rightchild:
			self.pTree(dashes+1, node.rightchild, 'R')
	"""

	
	def printTree(self, node, x):
		depth = ""
		if node == 0:
			node = self.root
		if node.rightchild:
			self.printTree(node.rightchild, x+1)
		for i in range(0, x):
			depth += '\t'
		if node.key:
			if node.leftchild and node.rightchild:	
				print(depth + str(node.key) + "\t\t\t(children: {}, {})".format(node.leftchild.key, node.rightchild.key))
			elif node.leftchild:
				print(depth + str(node.key) + "\t\t\t(children: {})".format(node.leftchild.key))
			elif node.rightchild:
				print(depth + str(node.key) + "\t\t\t(children: {})".format(node.rightchild.key))
			else:
				print(depth + str(node.key) + "\t\t\t(children: None)")
		if node.leftchild:
			self.printTree(node.leftchild, x+1)
	
	
	
	"""
	def printTree(self, node, depth):
		#print(node.key)
		dashes = ""
		try:
			printTreeStrings[depth] += dashes + " {}".format(node.key)
		except IndexError:
			printTreeStrings.append(str(node.key))
		if node.leftchild:
			self.printTree(node.leftchild, depth+1)
		if node.rightchild:
			self.printTree(node.rightchild, depth+1)
	"""


if __name__ == "__main__":

	while(1):
		command = raw_input('Command : ')
		command = command.split()
		print("")
		if command[0] == "Done":
			exit()
		elif command[0] == "BuildTree" and len(command) == 3:
			SCT = Tree(float (command[1]), int (command[2]))
		elif command[0] == "Insert" and len(command) == 2:
			SCT.insert(int (command[1]))
		elif command[0] == "Search" and len(command) == 2:
			SCT.search(int (command[1]))
		elif command[0] == "Delete" and len(command) == 2:
			SCT.delete(int (command[1]))
		elif command[0] == "Print":
			SCT.printTree(SCT.root, 0)
			"""
			for x in printTreeStrings:
				print(x)
			"""
		else:
			print("{}: incorrect format or command not recognized".format(command[0]))