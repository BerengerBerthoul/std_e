#if __cplusplus > 201703L
#include "std_e/graph/adjacency_graph/merge_from_leaves2.hpp"
#include "std_e/unit_test/doctest.hpp"

#include "std_e/graph/test_utils/io_graph.hpp"
#include "std_e/graph/adjacency_graph/adjacency_graph.hpp"


using namespace std_e;


struct S1 {
  char c;
  char h;
};
const auto eq_S = [](const S1& x, const S1& y){ return x.c == y.c; };
const auto less_S = [](const S1& x, const S1& y){ return x.c < y.c; };

auto
height(const S1& x) {
  return x.h;
}
auto
to_string(const S1& x) -> std::string {
  return {x.c};
}
auto
operator==(const S1& x, const S1& y) -> bool {
  return eq_S(x,y);
}
auto
operator<=>(const S1& x, const S1& y) {
  return x.c <=> y.c;
}

TEST_CASE("merge_from_leaves") {
  LOG("aaaaa-0");
  /*
                 F (10)
                / \
               /   \
              /     \
             /       \
            E (9)     \
           / \         \
          /   \         \
         /     \         \
        /       \         \
       D (6)     D (7)     D (8)
       /\        /\        /\
      /  \      /  \      /  \
     A    B    A    C    A    B
    (0)  (1)  (2)  (3)  (4)  (5)
  */
  auto g = make_io_adjacency_graph<S1>({
   /* 0*/ {S1{'A',0}, {}, {}    },
   /* 1*/ {S1{'B',0}, {}, {}    },
   /* 2*/ {S1{'A',0}, {}, {}    },
   /* 3*/ {S1{'C',0}, {}, {}    },
   /* 4*/ {S1{'A',0}, {}, {}    },
   /* 5*/ {S1{'B',0}, {}, {}    },

   /* 6*/ {S1{'D',1}, {}, {0,1} },
   /* 7*/ {S1{'D',1}, {}, {2,3} },
   /* 8*/ {S1{'D',1}, {}, {4,5} },

   /* 9*/ {S1{'E',2}, {}, {6,7} },
   /*10*/ {S1{'F',3}, {}, {9,8} },
  });
  make_bidirectional_from_outward_edges(g);

  SUBCASE("merge_from_leaves") {
    auto eq_ = [eq=eq_S](const auto& x, const auto& y){ return equivalent_by_node_and_outwards(x,y,eq); };
    auto less_ = [eq=eq_S,less=less_S](const auto& x, const auto& y){ return less_by_node_and_outwards(x,y,eq,less); };
    sort_redirect_inward_to_equivalent(begin(g),begin(g)+6,eq_,less_);
  }

  //SUBCASE("merge_from_leaves") {
  //  auto mg = merge_from_leaves2(g,eq_S,less_S);
  //  LOG("aaaaa05");

  //  /*
  //    Graph after factorization:
  //                 F (6)
  //                / \
  //               /   \
  //              /     \
  //             /      /
  //            E (5)  /
  //           / \    /
  //          /---\--/
  //         /     \
  //        /       \
  //       D (3)     D (4)
  //       /\        /\
  //      /  \      /  \
  //  (0)A    B    /    C
  //     \   (1)  /    (2)
  //      \------/
  //  */
  //  auto expected_g = make_io_adjacency_graph<S1>({
  //   /*0*/ {S1{'A',0}, {}, {}    },
  //   /*1*/ {S1{'B',0}, {}, {}    },
  //   /*2*/ {S1{'C',0}, {}, {}    },

  //   /*3*/ {S1{'D',1}, {}, {0,1} },
  //   /*4*/ {S1{'D',1}, {}, {0,2} },

  //   /*5*/ {S1{'E',2}, {}, {3,4} },
  //   /*6*/ {S1{'F',3}, {}, {5,3} },
  //  });
  //  make_bidirectional_from_outward_edges(expected_g);

  //  CHECK( mg == expected_g );
  //}
}
#endif // C++20