#include <gtest/gtest.h>
#include "../include/ClassScraper.hpp"

const std::string header = "test_files/headers/class.hpp";
const std::string source = "test_files/src/class.cpp";



sh_ptr<ClassScraper> class_scraper = std::make_shared<ClassScraper>(header, source);

TEST(ClassScraperTest, CHECK_INCLUDES) {
    vector<sh_ptr<ClassScraper::Block>> blocks = class_scraper->get_blocks();
    for (const auto& block : blocks) {
        if (dynamic_cast<ClassScraper::Include*>(block.get())) {
            ClassScraper::Include* include = dynamic_cast<ClassScraper::Include*>(block.get());
//          DEBUG("Include: " << include->get_body()[0])
//          DEBUG(" ----->" << include->get_base_depth() << "  " << include->reconstruct())
        }
    }
}