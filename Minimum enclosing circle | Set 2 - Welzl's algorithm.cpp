// C++ program to find the minimum enclosing
// circle for N integer points in a 2-D plane
#include <algorithm>
#include <assert.h>
#include <iostream>
#include <math.h>
#include <vector>
using namespace std;

// Defining infinity
const double INF = 1e18;

// Structure to represent a 2D point
struct Point {
	double X, Y;
};

// Structure to represent a 2D circle
struct Circle {
	Point C;
	double R;
};

// Function to return the euclidean distance
// between two points
double dist(const Point& a, const Point& b)
{
	return sqrt(pow(a.X - b.X, 2)
				+ pow(a.Y - b.Y, 2));
}

// Function to check whether a point lies inside
// or on the boundaries of the circle
bool is_inside(const Circle& c, const Point& p)
{
	return dist(c.C, p) <= c.R;
}

// The following two functions are used
// To find the equation of the circle when
// three points are given.

// Helper method to get a circle defined by 3 points
Point get_circle_center(double bx, double by,
						double cx, double cy)
{
	double B = bx * bx + by * by;
	double C = cx * cx + cy * cy;
	double D = bx * cy - by * cx;
	return { (cy * B - by * C) / (2 * D),
			(bx * C - cx * B) / (2 * D) };
}

// Function to return a unique circle that
// intersects three points
Circle circle_from(const Point& A, const Point& B,
				const Point& C)
{
	Point I = get_circle_center(B.X - A.X, B.Y - A.Y,
								C.X - A.X, C.Y - A.Y);

	I.X += A.X;
	I.Y += A.Y;
	return { I, dist(I, A) };
}

// Function to return the smallest circle
// that intersects 2 points
Circle circle_from(const Point& A, const Point& B)
{
	// Set the center to be the midpoint of A and B
	Point C = { (A.X + B.X) / 2.0, (A.Y + B.Y) / 2.0 };

	// Set the radius to be half the distance AB
	return { C, dist(A, B) / 2.0 };
}

// Function to check whether a circle
// encloses the given points
bool is_valid_circle(const Circle& c,
					const vector<Point>& P)
{

	// Iterating through all the points
	// to check whether the points
	// lie inside the circle or not
	for (const Point& p : P)
		if (!is_inside(c, p))
			return false;
	return true;
}

// Function to return the minimum enclosing
// circle for N <= 3
Circle min_circle_trivial(vector<Point>& P)
{
	assert(P.size() <= 3);
	if (P.empty()) {
		return { { 0, 0 }, 0 };
	}
	else if (P.size() == 1) {
		return { P[0], 0 };
	}
	else if (P.size() == 2) {
		return circle_from(P[0], P[1]);
	}

	// To check if MEC can be determined
	// by 2 points only
	for (int i = 0; i < 3; i++) {
		for (int j = i + 1; j < 3; j++) {

			Circle c = circle_from(P[i], P[j]);
			if (is_valid_circle(c, P))
				return c;
		}
	}
	return circle_from(P[0], P[1], P[2]);
}

// Returns the MEC using Welzl's algorithm
// Takes a set of input points P and a set R
// points on the circle boundary.
// n represents the number of points in P
// that are not yet processed.
Circle welzl_helper(vector<Point>& P,
					vector<Point> R, int n)
{
	// Base case when all points processed or |R| = 3
	if (n == 0 || R.size() == 3) {
		return min_circle_trivial(R);
	}

	// Pick a random point randomly
	int idx = rand() % n;
	Point p = P[idx];

	// Put the picked point at the end of P
	// since it's more efficient than
	// deleting from the middle of the vector
	swap(P[idx], P[n - 1]);

	// Get the MEC circle d from the
	// set of points P - {p}
	Circle d = welzl_helper(P, R, n - 1);

	// If d contains p, return d
	if (is_inside(d, p)) {
		return d;
	}

	// Otherwise, must be on the boundary of the MEC
	R.push_back(p);

	// Return the MEC for P - {p} and R U {p}
	return welzl_helper(P, R, n - 1);
}

Circle welzl(const vector<Point>& P)
{
	vector<Point> P_copy = P;
	random_shuffle(P_copy.begin(), P_copy.end());
	return welzl_helper(P_copy, {}, P_copy.size());
}

// Driver code
int main()
{
	Circle mec = welzl({ { 0, 0 },
						{ 0, 1 },
						{ 1, 0 } });
	cout << "Center = { " << mec.C.X << ", " << mec.C.Y
		<< " } Radius = " << mec.R << endl;

	Circle mec2 = welzl({ { 5, -2 },
						{ -3, -2 },
						{ -2, 5 },
						{ 1, 6 },
						{ 0, 2 } });
	cout << "Center = { " << mec2.C.X << ", " << mec2.C.Y
		<< " } Radius = " << mec2.R << endl;

	return 0;
}
