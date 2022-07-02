#include <chrono>
#include <iostream>

#include "curve.h"
#include "curve_creator.h"
#include "curve_segment.h"

using namespace reference_line;

void test_seg() {
  double theta0 = M_PI / 6;
  double kappa0 = 0;
  double dkappa0 = 0;
  double x0 = 0;
  double y0 = 0;
  double theta1 = -M_PI / 6;
  double kappa1 = 0;
  double dkappa1 = 0;
  double x1 = 0;
  double y1 = 0;
  double delta = 1;
  reference_line::CurveSegment seg(theta0, kappa0, dkappa0, x0, y0, theta1,
                                   kappa1, dkappa1, x1, y1, delta);
  printf("%f,%f\n", seg.x(1), seg.y(1));
}


void test_curve() {
  std::vector<double> xs{0, 1, 2};
  std::vector<double> ys{0, 0.3, 0};
  reference_line::Curve curve(xs, ys);
}

/**
 * @brief 等距弧长离散园
 * 由于圆的特殊性，其圆弧上每一点的曲率均相等，故可以将等弧长与相同的角度相对应
 * @param  x
 * @param  y
 * @param  r
 * @param  size
 */
void div_circle(std::vector<double>& x, std::vector<double>& y, double r,
                double size)  // xy对应圆心坐标,r为半径,size用于设置划分的间距
{
  double angle_step = 0.0;  //一小步的弧度
  angle_step = size / r;
  double x_out, y_out;

  for (int i = M_PI / angle_step; i > 0; --i) {
    x_out = r * std::cos(i * angle_step);
    y_out = r * std::sin(i * angle_step);
    x.push_back(x_out);
    y.push_back(y_out);
  }
}

void test_solve() {
  // 测试数据
  /*
    std::vector<double> raw_xs{
        4.946317773,   5.017218975,   4.734635316,  4.425064575,  3.960102096,
        3.503172702,   2.989950824,   2.258523535,  1.562447892,  0.8764776599,
        0.09899323097, -0.7132021974, -1.479055426, -2.170306775, -3.034455492,
        -3.621987909,  -3.979289889,  -4.434628966, -4.818245921, -4.860190444,
        -5.09947597};
    std::vector<double> raw_ys{
        0.08436953512, 0.7205757236, 1.642930209, 2.365356462, 2.991632152,
        3.44091492,    3.9590821,    4.354377368, 4.46801472,  4.871705856,
        5.0285845841,  5.010851105,  4.680181989, 4.463442715, 4.074651273,
        3.585790302,   3.014232351,  2.367848826, 1.467395733, 0.8444358019,
        -0.01022405467};
  */

  std::vector<double> raw_xs, raw_ys;
  div_circle(raw_xs, raw_ys, 7, 1);

  reference_line::CurveCreator creator(raw_xs, raw_ys);

  creator.solve();

/*
  auto time1 = std::chrono::system_clock::now();
  auto time2 = std::chrono::system_clock::now();
  std::chrono::duration<double> diff = time2 - time1;
  std::cout << "Time for solve time = " << diff.count() * 1000 << " msec.\n";
*/

  auto& curve = creator.curve();
  double length = curve->length();

  // constexpr int N = 44;
  int N = 200;

  double stride = length / N;

  std::vector<double> final_xs, final_ys, final_kappas, final_dkappas,
      final_deltas;
  final_deltas = curve->deltas_;

  for (double i = 0; i < N + 1; i++) {
    final_xs.emplace_back(curve->x(i * stride));
    final_ys.emplace_back(curve->y(i * stride));
    final_kappas.emplace_back(curve->kappa(i * stride));
    final_dkappas.emplace_back(curve->dkappa(i * stride));
  }
}

int main() {
  std::cout << "Hello, World!" << std::endl;
  test_solve();

  return 0;
}
