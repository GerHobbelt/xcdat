#include <xcdat.hpp>

#include "cmd_line_parser/parser.hpp"
#include "mm_file/mm_file.hpp"
#include "tinyformat/tinyformat.h"

cmd_line_parser::parser make_parser(int argc, char** argv) {
    cmd_line_parser::parser p(argc, argv);
    p.add("input_idx", "Input filepath of trie index");
    return p;
}

template <class Trie>
int decode(const cmd_line_parser::parser& p) {
    const auto input_idx = p.get<std::string>("input_idx");

    const mm::file_source<char> fin(input_idx.c_str(), mm::advice::sequential);
    const auto trie = xcdat::mmap<Trie>(fin.data());

    for (std::uint64_t id; std::cin >> id;) {
        const auto dec = trie.decode(id);
        tfm::printfln("%d\t%s", id, dec);
    }

    return 0;
}

int main(int argc, char** argv) {
#ifndef NDEBUG
    tfm::warnfln("The code is running in debug mode.");
#endif
    std::ios::sync_with_stdio(false);

    auto p = make_parser(argc, argv);
    if (!p.parse()) {
        return 1;
    }

    const auto input_idx = p.get<std::string>("input_idx");
    const auto flag = xcdat::get_flag(input_idx);

    switch (flag) {
        case 7:
            return decode<xcdat::trie_7_type>(p);
        case 8:
            return decode<xcdat::trie_8_type>(p);
        default:
            break;
    }

    p.help();
    return 1;
}