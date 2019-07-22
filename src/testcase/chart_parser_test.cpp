#include <gtest/gtest.h>
#include "chart_parser.h"

TEST(chart_parser_test, test1) {
    std::shared_ptr<XDocManager> dm{new XDocManager{QDir{"../testcases"}, QDir{"../testcase-templates"}}};
    auto m = parseChart("just-idle", dm);
    // auto doc = dm->getDoc("just-idle");
    // ChartParser p;
    // p.xdocManager = dm;
    // p.chartId = "just-idle";
    // bool ok = p.parse(*doc);

    ASSERT_TRUE(m != nullptr);

    m->dumpObjectTree();
}