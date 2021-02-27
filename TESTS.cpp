//
// Created by cirkul on 08.12.2020.
//
#include "iostream"
#include "catch.hpp"
#include "Graph.h"


TEST_CASE() {
    SECTION("1. node brute force") {
        Graph<int, int> graph;
        graph.createTheGraph("graph_config.txt");
        graph.fillTheGraph("graph_setup.txt");
        std::vector<int> curr;
        std::vector<int> required = {1, 2, 3, 4, 5};
        for (auto item : graph)
            curr.push_back(*item.data);
        REQUIRE(curr == required);
    }SECTION("2. arc brute force") {
        Graph<int, int> graph;
        graph.createTheGraph("graph_config.txt");
        graph.fillTheGraph("graph_setup.txt");
        std::vector<int> curr;
        std::vector<int> required = {1, 2, 3, 4, 5, 6};
        for (auto it = graph.beginArc(); it != graph.endArc(); ++it)
            curr.push_back(*it->data);
        REQUIRE(curr == required);
    }SECTION("3. adjacent brute force") {
        Graph<int, int> graph;
        graph.createTheGraph("graph_config.txt");
        graph.fillTheGraph("graph_setup.txt");
        std::vector<int> curr;
        std::vector<int> required = {2, 4, 3, 3, 5, 1};
        for (auto item : graph)
            for (auto it = item.beginAdjacentNode(); it != item.endAdjacentNode(); ++it)
                curr.push_back(*it->data);

        REQUIRE(curr == required);
    }SECTION("4. incidental brute force") {
        Graph<int, int> graph;
        graph.createTheGraph("graph_config.txt");
        graph.fillTheGraph("graph_setup.txt");
        std::vector<int> curr;
        std::vector<int> required = {1, 2, 6, 1, 3, 3, 4, 2, 4, 5, 5, 6};
        for (auto item : graph)
            for (auto it = item.beginIncidentalArcs(); it != item.endIncidentalArcs(); ++it)
                curr.push_back(*it->data);

        REQUIRE(curr == required);
    }SECTION("5. rnode brute force") {
        Graph<int, int> graph;
        graph.createTheGraph("graph_config.txt");
        graph.fillTheGraph("graph_setup.txt");
        std::vector<int> curr;
        std::vector<int> required = {5, 4, 3, 2, 1};
        for (auto it = graph.rbegin(); it != graph.rend(); ++it)
            curr.push_back(*it->data);
        REQUIRE(curr == required);
    }SECTION("6. rarc brute force") {
        Graph<int, int> graph;
        graph.createTheGraph("graph_config.txt");
        graph.fillTheGraph("graph_setup.txt");
        std::vector<int> curr;
        std::vector<int> required = {6, 5, 4, 3, 2, 1};
        for (auto it = graph.rbeginArc(); it != graph.rendArc(); ++it)
            curr.push_back(*it->data);
        REQUIRE(curr == required);
    }SECTION("7. radjacent brute force") {
        Graph<int, int> graph;
        graph.createTheGraph("graph_config.txt");
        graph.fillTheGraph("graph_setup.txt");
        std::vector<int> curr;
        std::vector<int> required = {4, 2, 3, 5, 3, 1};

        for (auto item : graph)
            for (auto it = item.rbeginAdjacentNode(); it != item.rendAdjacentNode(); ++it)
                curr.push_back(*it->data);

        REQUIRE(curr == required);
    }SECTION("8. rincidental brute force") {
        Graph<int, int> graph;
        graph.createTheGraph("graph_config.txt");
        graph.fillTheGraph("graph_setup.txt");
        std::vector<int> curr;
        std::vector<int> required = {6, 2, 1, 3, 1, 4, 3, 5, 4, 2, 6, 5};
        for (auto item : graph)
            for (auto it = item.rbeginIncidentalArcs(); it != item.rendIncidentalArcs(); ++it)
                curr.push_back(*it->data);

        REQUIRE(curr == required);
    }SECTION("9. const node brute force") {
        Graph<int, int> graph;
        graph.createTheGraph("graph_config.txt");
        graph.fillTheGraph("graph_setup.txt");
        Graph<int, int> cgraph(graph);
        std::vector<int> curr;
        std::vector<int> required = {1, 2, 3, 4, 5};
        for (auto it = cgraph.cbegin(); it != cgraph.cend(); ++it)
            curr.push_back(*it->data);

        REQUIRE(curr == required);
    }SECTION("10. const arc brute force") {
        Graph<int, int> graph;
        graph.createTheGraph("graph_config.txt");
        graph.fillTheGraph("graph_setup.txt");
        Graph<int, int> cgraph(graph);
        std::vector<int> curr;
        std::vector<int> required = {1, 2, 3, 4, 5, 6};
        for (auto it = cgraph.cbeginArc(); it != cgraph.cendArc(); ++it)
            curr.push_back(*it->data);
        REQUIRE(curr == required);
    }SECTION("11. const adjacent brute force") {
        Graph<int, int> graph;
        graph.createTheGraph("graph_config.txt");
        graph.fillTheGraph("graph_setup.txt");
        Graph<int, int> cgraph(graph);
        std::vector<int> curr;
        std::vector<int> required = {2, 4, 3, 3, 5, 1};

        for (auto it = cgraph.cbegin(); it != cgraph.cend(); ++it)
            for (auto it1 = it->cbeginAdjacentNode(); it1 != it->cendAdjacentNode(); ++it1)
                curr.push_back(*it1->data);

        REQUIRE(curr == required);
    }SECTION("12. const incidental brute force") {

        Graph<int, int> graph;
        graph.createTheGraph("graph_config.txt");
        graph.fillTheGraph("graph_setup.txt");
        const Graph<int, int> cgraph = graph;
        std::vector<int> curr;
        std::vector<int> required = {1, 2, 6, 1, 3, 3, 4, 2, 4, 5, 5, 6};
        for (auto it = cgraph.cbegin(); it != cgraph.cend(); ++it)
            for (auto it1 = it->cbeginIncidentalArcs(); it1 != it->cendIncidentalArcs(); ++it1)
                curr.push_back(*it1->data);

        REQUIRE(curr == required);
    } SECTION("13. empty & clear") {
        Graph<int, int> graph;
        graph.createTheGraph("graph_config.txt");
        graph.fillTheGraph("graph_setup.txt");
        REQUIRE_FALSE(graph.empty());
        graph.clear();
        REQUIRE(graph.empty());
    } SECTION("14. add") {
        Graph<int, int> graph;
        graph.createTheGraph("graph_config.txt");
        graph.fillTheGraph("graph_setup.txt");
        graph.addNode(5);
        std::vector<int> curr;
        std::vector<int> required = {1, 2, 3, 4, 5, 5};
        for (auto item : graph)
            curr.push_back(*item.data);
        REQUIRE(curr == required);
    } SECTION("15. erase") {
        Graph<int, int> graph;
        graph.createTheGraph("graph_config.txt");
        graph.fillTheGraph("graph_setup.txt");
        graph.eraseArc(graph.beginArc());
        std::vector<int> curr;
        std::vector<int> required1 = {2, 3, 4, 5, 6};
        std::vector<int> required2 = {2, 3, 4, 5};
        for (auto it = graph.beginArc(); it != graph.endArc(); ++it)
            curr.push_back(*it->data);
        REQUIRE(curr == required1);
        curr.clear();
        graph.eraseNode(graph.begin());
        for (auto item : graph)
            curr.push_back(*item.data);
        REQUIRE(curr == required2);
    } SECTION("16. linked") {
        Graph<int, int> graph;
        graph.createTheGraph("graph_config.txt");
        graph.fillTheGraph("graph_setup.txt");
        auto it1 = graph.begin();
        auto it2 = graph.begin();
        REQUIRE_FALSE(graph.linked(it1, it2));
        ++it2;
        REQUIRE(graph.linked(it1, it2));
        ++it2;
        REQUIRE_FALSE(graph.linked(it1, it2));
    } SECTION("17. power & el count") {
        Graph<int, int> graph;
        graph.createTheGraph("graph_config.txt");
        graph.fillTheGraph("graph_setup.txt");
        REQUIRE(graph.nodePower(graph.begin()) == 3);
        REQUIRE(graph.getNodeCount() == 5);
        REQUIRE(graph.getArcCount() == 6);
    } SECTION("18. find") {
        Graph<int, int> graph;
        graph.createTheGraph("graph_config.txt");
        graph.fillTheGraph("graph_setup.txt");
        auto it = graph.begin();
        for (int i = 0; i < 3; ++i) ++it;
        auto it2 = graph.find(4);
        REQUIRE(it.node == it2.node);
    }
}
