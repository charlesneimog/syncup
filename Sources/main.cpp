#include <cmath>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

// ─────────────────────────────────────
// From
// https://stackoverflow.com/questions/56788745/how-to-convert-stdfilesystemfile-time-type-to-a-string-using-gcc-9
template <typename TP> std::time_t to_time_t(TP tp) {
    using namespace std::chrono;
    auto sctp =
        time_point_cast<system_clock::duration>(tp - TP::clock::now() + system_clock::now());
    return system_clock::to_time_t(sctp);
}

// ─────────────────────────────────────
void print_time(std::string Id, std::time_t Time) {
    std::tm *gmt = std::gmtime(&Time);
    std::cout << Id << " " << std::put_time(gmt, "%A, %d %B %Y %H:%M:%S") << std::endl;
}

// ─────────────────────────────────────
int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cout << "Use: syncup <source> <destination>" << std::endl;
        return 1;
    }

    fs::path Source = argv[1];
    fs::path Destination = argv[2];

    // TODO: Need to delete files that are not in the source directory.

    if (fs::exists(Source) && fs::is_directory(Source)) {
        for (const auto &Entry : fs::recursive_directory_iterator(Source)) {
            auto SourceTime = fs::last_write_time(Entry.path());
            if (fs::is_regular_file(Entry.path())) {
                fs::path RelativePath = Entry.path().lexically_relative(Source);
                fs::path DestinationPath = Destination / RelativePath;
                if (fs::exists(DestinationPath)) {
                    auto DestinationTime = fs::last_write_time(DestinationPath);
                    const long SourceTimeT = to_time_t(SourceTime);
                    const long DestinationTimeT = to_time_t(DestinationTime);
                    int Diff = std::abs(SourceTimeT - DestinationTimeT);

                    if (Diff > 5) {
                        fs::copy_file(Entry.path(), DestinationPath,
                                      fs::copy_options::update_existing);
                        fs::last_write_time(DestinationPath, SourceTime);
                        std::cout << "Updating: " << RelativePath << std::endl;
                        print_time("Src Time:", SourceTimeT);
                        print_time("Dst Time:", DestinationTimeT);
                        printf("\n");
                    }

                } else {
                    fs::create_directories(DestinationPath.parent_path());
                    fs::copy_file(Entry.path(), DestinationPath, fs::copy_options::update_existing);
                    fs::last_write_time(DestinationPath, SourceTime);
                    std::cout << "Creating: " << DestinationPath << std::endl;
                    printf("\n");
                }
            }
        }
    } else {
        std::cout << "The directory does not exist." << std::endl;
    }

    // Search for source files that were deleted from Sources
    for (const auto &Entry : fs::recursive_directory_iterator(Destination)) {
        if (fs::is_regular_file(Entry.path())) {
            fs::path RelativePath = Entry.path().lexically_relative(Destination);
            fs::path SourcePath = Source / RelativePath;
            if (!fs::exists(SourcePath)) {
                fs::remove(Entry.path());
                std::cout << "Deleting: " << Entry.path() << std::endl;
            }
        }
    }

    return 0;
}
