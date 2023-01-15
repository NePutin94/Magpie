#ifndef MAGPIE_MYIMGUI_H
#define MAGPIE_MYIMGUI_H

#include <imgui.h>
#include <set>
#include <map>
#include <filesystem>
#include <Texture.h>

namespace ImGui
{
    bool SelectableImage(const palka::Texture& texture, bool selected, const ImVec2& size);

    class FileManager_Context;

    std::pair<bool, std::string> FileManager(FileManager_Context& context);

    class FileManager_Context
    {
    private:
        bool open;
        bool updateDir;
        std::string _path;
        std::string filter;
        std::set<int> selected_nodes;
        std::multimap<size_t, std::filesystem::directory_entry> files;
        static long id;
        int curr_id;
        bool Input;
        bool selectOne;

        bool windowIsFocus;
    public:
        FileManager_Context() = default;
        FileManager_Context(std::string _path, bool selectOne = false, std::string filter = "") : selectOne(selectOne), Input(false),
                                                                                                    _path(_path), curr_id(++id),
                                                                                                    open(false), filter(filter),
                                                                                                    updateDir(true), windowIsFocus(false)
        {}

        std::string getPath()
        { return _path; }

        void setOpen(bool _open)
        { open = (open) ? true : _open; }

        friend std::pair<bool, std::string> ImGui::FileManager(FileManager_Context&);

    };
}

#endif //MAGPIE_MYIMGUI_H
