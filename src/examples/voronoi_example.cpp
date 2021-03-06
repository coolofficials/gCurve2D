//#include <CGAL/Simple_cartesian.h>
//#include <cassert>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Delaunay_triangulation_adaptation_policies_2.h>
#include <CGAL/Delaunay_triangulation_adaptation_traits_2.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Voronoi_diagram_2.h>
#include <CGAL/point_generators_2.h>
#include <algorithm>
#include <vector>
//#include <CGAL/algorithm.h>
//#include <CGAL/random_selection.h>
using namespace CGAL;
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point;
typedef Creator_uniform_2<double, Point> Creator;
typedef std::vector<Point> Vector;

typedef CGAL::Delaunay_triangulation_2<K> DT;
typedef CGAL::Delaunay_triangulation_adaptation_traits_2<DT> AT;
typedef CGAL::Delaunay_triangulation_caching_degeneracy_removal_policy_2<DT> AP;
typedef CGAL::Voronoi_diagram_2<DT, AT, AP> VD;

int main() {

  // 1. VD to DT

  // Create test point set.
  Vector points;
  // Create 10 points within a disc of radius 150.
  Random_points_in_disc_2<Point, Creator> g(150.0);
  std::copy_n(g, 10, std::back_inserter(points));

  // Create VD.
  VD vd(points.begin(), points.end());
  // debug
  for (auto i = vd.sites_begin(); i != vd.sites_end(); i++) {
    std::cout << *i << std::endl; // == points
  }
  std::cout << std::endl;

  // Dual DT of VD
  DT dt = vd.dual();
  // debug
  for (auto i = dt.vertices_begin(); i != dt.vertices_end(); i++) {
    std::cout << i->point() << std::endl;
  }
  std::cout << std::endl;

  // 2. DT to VD

  Vector points2;
  for (auto i = dt.vertices_begin(); i != dt.vertices_end(); i++) {
    points2.push_back(i->point());
  }
  VD vd2(points2.begin(), points2.end());

  std::cout << "------------------------" << std::endl;

  // Requisites of Crust
  // An edge of DT belongs to the crust of S if both its endpoints belong to S.
  for (auto i = dt.all_edges_begin(); i != dt.all_edges_end(); i++) {
    auto face = i->first;
    auto index = i->second;
    auto source = face->vertex(CGAL::Triangulation_cw_ccw_2::cw(index));
    auto target = face->vertex(CGAL::Triangulation_cw_ccw_2::ccw(index));
    std::cout << *source << " " << *target;

    int source_in_S = std::find(points.begin(), points.end(),
                                source->point()) != points.end();
    int target_in_S = std::find(points.begin(), points.end(),
                                target->point()) != points.end();

    if (source_in_S && target_in_S) {
      std::cout << " S contains both endpoints";
    } else {
      std::cout << " S does not contain at least one endpoint";
    }

    std::cout << std::endl;
  }

  return 0;
}