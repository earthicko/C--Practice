#include <cmath>
#include <iostream>

class Point {
   public:
    int x, y;
    Point(int pos_x, int pos_y) {
        x = pos_x;
        y = pos_y;
    }
    void print_coord() {
        std::cout << "(" << x << ", " << y << ")" << std::endl;
    }
    double get_distance(const Point& another) {
        double output = sqrt(pow((another.x - x), 2) + pow((another.y - y), 2));
        return output;
    }
};

class Line {
   private:
    Point* p1;
    Point* p2;
    double incline, y_b;
    double a, b, c;

   public:
    Point* get_p1() { return p1; }
    Point* get_p2() { return p2; }
    Line(Point* _p1, Point* _p2) {
        p1 = _p1;
        p2 = _p2;

        incline = (double)(_p1->y - _p2->y) / (double)(_p1->x - _p2->x);
        y_b = (double)(_p2->x * _p1->y - _p1->x * _p2->y) /
              (double)(_p2->x - _p1->x);

        a = incline;
        b = -1;
        c = y_b;
    }
    double input_point(const Point& point) {
        double output = a * point.x + b * point.y + c;
        return output;
    }
    void print_info() {
        std::cout << a << "x+" << b << "y+" << c << "=0 or "
                  << "y=" << incline << "x+" << y_b << std::endl;
    }
};

class Geometry {
   private:
    int num_points;
    Point* points[10];
    // points is array of 100 pointer to Point
    Line* lines[10][10];
    // lines is array of 10 array of 10 pointer to Line

   public:
    Geometry() { num_points = 0; }
    void add_point(const Point& point) {
        points[num_points] = new Point(point.x, point.y);
        for (int i = 0; i < num_points; i++) {
            lines[i][num_points] = new Line(points[i], points[num_points]);
        }
        num_points++;
    }

    // 모든 점들 간의 거리를 출력하는 함수 입니다.
    void print_distance();
    // 모든 점들을 잇는 직선들 간의 교점의 수를 출력해주는 함수 입니다.
    // 참고적으로 임의의 두 점을 잇는 직선의 방정식을 f(x,y) = ax+by+c = 0
    // 이라고 할 때 임의의 다른 두 점 (x1, y1) 과 (x2, y2) 가 f(x,y)=0 을
    // 기준으로 서로 다른 부분에 있을 조건은 f(x1, y1) * f(x2, y2) <= 0 이면
    // 됩니다.
    void print_num_meetings();
    void print_points() {
        for (int i = 0; i < num_points; i++) {
            std::cout << "Point " << i << ": ";
            points[i]->print_coord();
        }
    }
    void print_lines() {
        for (int i = 0; i < num_points - 1; i++) {
            for (int j = i + 1; j < num_points; j++) {
                std::cout << "Pt[" << i << "] to Pt[" << j << "]: ";
                lines[i][j]->print_info();
            }
        }
    }
};

void Geometry::print_distance() {
    double dist_sum = 0;
    for (int i = 0; i < num_points; i++) {
        for (int j = i + 1; j < num_points; j++) {
            double dist = points[i]->get_distance(*(points[j]));
            dist_sum += dist;
            std::cout << "Pt[" << i << "]"
                      << " to Pt[" << j << "]: " << dist << std::endl;
        }
    }
    std::cout << "Sum: " << dist_sum << std::endl;
}

void Geometry::print_num_meetings() {
    int itsc_count = 0;
    for (int i = 0; i < num_points - 1; i++) {
        for (int j = i + 1; j < num_points; j++) {
            Line* test = lines[i][j];
            for (int i_re = i; i_re < num_points - 1; i_re++) {
                for (int j_re = i_re + 1; j_re < num_points; j_re++) {
                    if (i_re <= i && j_re <= j) continue;
                    Line* target = lines[i_re][j_re];
                    double result1 = test->input_point(*target->get_p1());
                    double result2 = test->input_point(*target->get_p2());
                    if (result1 * result2 < 0) {
                        itsc_count++;
                        std::cout << "Itsc found between pt[" << i << "]<->pt["
                                  << j << "] and pt[" << i_re << "]<->pt["
                                  << j_re << "]" << std::endl;
                    }
                }
            }
        }
    }
    std::cout << itsc_count << " itsc found." << std::endl;
}

int main() {
    Geometry geometry;
    geometry.add_point(Point(1, 3));
    geometry.add_point(Point(5, 1));
    geometry.add_point(Point(7, 2));
    geometry.add_point(Point(6, 5));
    geometry.add_point(Point(4, 8));
    geometry.add_point(Point(2, 6));

    geometry.print_points();
    geometry.print_lines();

    geometry.print_distance();
    geometry.print_num_meetings();

    return 0;
}