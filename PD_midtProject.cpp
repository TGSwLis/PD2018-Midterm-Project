#include <iostream>
#include <cmath>
using namespace std;

void input(int [], int m);
double disBetween(int x1, int y1, int x2, int y2); // calculate the distance between two points
double partDiff(int start, int end, double d); // calculate the difference between each partition
void currentLoc(double* now, double move); // find the current location of the jet
bool checkRoute(int startX, int startY, int endX, int endY, int x, int y); // check if the turning point is on the route directly from the starting point to the ending point
double threatDistance(double x, double y, int xt, int yt, int r); // calculate the distance to the threat point

int main() {
	// data input
	int n = 0, threatNum = 0, turnCost = 0, fuelLimit = 0;
	cin >> n >> threatNum >> turnCost >> fuelLimit;
	
	int* xLoc = new int[threatNum]; // the x coordinates of the threat points
	input(xLoc, threatNum);
	int* yLoc = new int[threatNum]; // the y coordinates of the threat points
	input(yLoc, threatNum);
	int* rad = new int[threatNum]; // the radii of the threat points
	input(rad, threatNum);
	int* p = new int[threatNum]; // the threating extent of the threat points
	input(p, threatNum);
	
	int start[2] = {0}, end[2] = {0}; // the starting point & the end point
	input(start, 2);
	input(end, 2);
	
	// calculate the threat directly from starting point to end point
	// (k = 1)
	int turn = 0; // record how many times the jet turns 
	double distance0 = disBetween(start[0], start[1], end[0], end[1]);
	double xMove0 = partDiff(start[0], end[0], distance0); // record x difference between two parts
	double yMove0 = partDiff(start[1], end[1], distance0); // record y difference between two parts
	int part0 = static_cast<int>(distance0); // record how many parts in the route
	if(part0 != distance0) // distance might not be an integer
		part0 += 1;
	
	double minThreat = 0;
	double xNow0 = start[0], yNow0 = start[1]; // records the current location
	
	for(int i = 0; i < part0 - 1; i++) { // for each part
		// calculate current location
		currentLoc(&xNow0, xMove0);
		currentLoc(&yNow0, yMove0);
		
		for(int j = 0; j < threatNum; j++) // for each threat point
			minThreat += p[j] * threatDistance(xNow0, yNow0, xLoc[j], yLoc[j], rad[j]); 
	}
	
/*	// test
	cout << distance0 << endl;
	cout << xMove0 << " " << yMove0 << endl; 
	cout << minThreat; */

	// turn once 
	// (k = 2)
	int turnX = 0, turnY = 0; // record (x, y) of the turning point
	
	// go through the whole map
	for(int i = 0; i < n; i++) { // x
		for(int j = 0; j < n; j++) { // y
			double threat = 0;
			
			// turning point should not be on the route directly from the starting point to the ending point
			bool check = checkRoute(start[0], start[1], end[0], end[1], i , j);
			if(check == true)
				continue;
			
			// calculate the distance between starting point & turning point + the distance between turning point & ending point
			double distance1 = disBetween(start[0], start[1], i, j);
			double distance2 = disBetween(i, j, end[0], end[1]);
			
			// the total distance should be less than or equal to fuel limit
			if(distance1 + distance2 > fuelLimit)
				continue;
			
			// find the partitions in the route
			int partSum = static_cast<int>(distance1 + distance2);
			if(partSum != distance1 + distance2) 
				partSum += 1;
			int part1 = static_cast<int>(distance1);
			int part2 = partSum - part1;
			
			double xMove1 = partDiff(start[0], i, distance1);
			double yMove1 = partDiff(start[1], j, distance1);
			double xNow1 = start[0], yNow1 = start[1];
			
			for(int s = 0; s < part1; s++) { // for each part
				// calculate current location
				currentLoc(&xNow1, xMove1);
				currentLoc(&yNow1, yMove1);
		
				for(int t = 0; t < threatNum; t++) // for each threat point
					threat += p[t] * threatDistance(xNow1, yNow1, xLoc[t], yLoc[t], rad[t]); 
			}
			
			double remain = 1 - disBetween(xNow1, yNow1, i, j);
			double xMove2 = partDiff(i, end[0], distance2);
			double yMove2 = partDiff(j, end[1], distance2);
			xNow1 = i + remain * xMove2;
			yNow1 = j + remain * yMove2;
			
			for(int s = 0; s < part2 - 1; s++) { // for each part
				for(int t = 0; t < threatNum; t++) // for each threat point
					threat += p[t] * threatDistance(xNow1, yNow1, xLoc[t], yLoc[t], rad[t]); 
				
				// move to the next location
				currentLoc(&xNow1, xMove2);
				currentLoc(&yNow1, yMove2);
			}
			
			// determine whether the threat is the minimum threat
			threat += turnCost;
			if(threat < minThreat) {
				turn = 1;
				minThreat = threat;
				turnX = i;
				turnY = j;
			}
		}
	} 
	
	// should be written as a function
	if(turn == 0)
		cout << turn;
	else cout << turn << " " << turnX << " " << turnY; 	
	
	// release memory
	delete [] xLoc;
	delete [] yLoc;
	delete [] rad;
	delete [] p; 
	
	return 0;
} 

void input(int a[], int m) {
	for(int i = 0; i < m; i++)
		cin >> a[i];
}

double disBetween(int x1, int y1, int x2, int y2) {
	return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

double partDiff(int start, int end, double d) {
	return (end - start) / d;
}

void currentLoc(double* now, double move) {
	*now += move;
}

bool checkRoute(int startX, int startY, int endX, int endY, int x, int y) {
	double xDelta = endX - startX;
	double yDelta = endY - startY;	
	double slope = yDelta / xDelta;
	
	double xDiff = x - startX;
	double yDiff = y - startY;
	
	if(xDiff * slope == yDiff) {
		if(xDiff / xDelta >= 0 && xDiff / xDelta <= 1) {
			if(yDiff / yDelta >= 0 && yDiff / yDelta <= 1) {
				return true;
			}
		}
	}
	
	return false;
}

double threatDistance(double x, double y, int xt, int yt, int r) {
	double d = sqrt((x - xt) * (x - xt) + (y - yt) * (y - yt));
	double threatD = (r - d) / r;	
	
	if(threatD > 0) 
		return threatD;
	else return 0;
}
