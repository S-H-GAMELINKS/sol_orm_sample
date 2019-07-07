#include "cpp-httplib/httplib.h"
#include "sqlite_orm/include/sqlite_orm/sqlite_orm.h"
#include <fstream>
#include <sstream>
#include <string>

struct Post{
    int id;
    std::string content;
};

using namespace sqlite_orm;

const std::string load_static(const std::string& path) {

    std::ifstream static_file(path.c_str(), std::ios::in);

    std::stringstream stream;

    stream << static_file.rdbuf();
    static_file.close();

    return stream.str();
}

int main() {

    httplib::Server svr;

    auto storage = make_storage("db.sqlite",
                            make_table("posts",
                                       make_column("id", &Post::id, autoincrement(), primary_key()),
                                       make_column("content", &Post::content));

    const std::string html = load_static("static/index.html"); 

    const std::string js = load_static("static/index.js"); 

    svr.Get("/", [&](const httplib::Request& req, httplib::Response& res){
        res.set_content(html, "text/html");
    });

    svr.Get("/index.js", [&](const httplib::Request& req, httplib::Response& res){
        res.set_content(js, "text/javascript");
    });

    svr.Post("/api/posts", [&](const httplib::Request, &req, httplib::Response& res){
        decltype(auto) content = req.params.find("content");
        Post post{-1, content};
        storage.insert(post);
        res.set_content("OK", "text/plain");
    });

    svr.Get("/api/posts", [&](const httplib::Request, &req, httplib::Response& res){
        auto allPosts = storage.get_all<Post>();
        std::string response = "";

        for(auto&& post : allPosts)
            response += storage.dump(post);

        res.set_content(response.c_str(), "text/json");
    })

    svr.listen("localhost", 3000);
}