#pragma once
namespace fs = boost::filesystem;


namespace Utility
{
    typedef boost::function<bool(const fs::path&)> Pred;

    bool is_parent_path( const fs::path& parent, const fs::path& child );
    bool remove_file( const fs::path& file_path );
    bool rename_file( const fs::path& old_path, const fs::path& new_path );

    template<typename Continer>
    void get_files_of_directory_if_impl( Continer& continer, const fs::path& dir, const Pred& predicate = Pred(), size_t first_n = -1 )
    {
        continer.clear();
        size_t n = 0;

        if ( ! exists( dir ) || ! is_directory( dir ) )
        {
            return;
        }

        std::back_insert_iterator<Continer> inserter( continer );
        bool is_pred_empty = predicate.empty();
        fs::recursive_directory_iterator end;
        for ( fs::recursive_directory_iterator it( dir ); it != end; ++it )
        {
            if ( ! is_directory( it->status() ) )
            {
                if ( ! is_pred_empty && ! predicate( it->path() ) )
                {
                    continue;
                }

                *inserter++ = it->path();

                if ( first_n <= ++n )
                {
                    return;
                }
            }
        }
    }

    std::vector<fs::path> get_files_of_directory( const fs::path& dir );
    std::vector<fs::path> get_files_of_directory_n( const fs::path& dir, size_t first_n );
    std::vector<fs::path> get_files_of_directory_if( const fs::path& dir, const Pred& pred );
    std::vector<fs::path> get_files_of_directory_if_n( const fs::path& dir, const Pred& pred, size_t first_n );
    std::list<fs::path> get_file_list_of_directory( const fs::path& dir );
    std::list<fs::path> get_file_list_of_directory_n( const fs::path& dir, size_t first_n );
    std::list<fs::path> get_file_list_of_directory_if( const fs::path& dir, const Pred& pred );
    std::list<fs::path> get_file_list_of_directory_if_n( const fs::path& dir, const Pred& pred, size_t first_n );
}
