#ifndef MAGPIE_ICOHOLDER_H
#define MAGPIE_ICOHOLDER_H

#include <map>
#include <Texture.h>

namespace Magpie
{
    enum class ico_t
    {
        Editor_Delete = 0,
        Editor_Add,
        Folder,
        File_img,
        File_txt,
        Refresh,
        Magic,
        Save,
        Create,
        Move,
        Hand,
        Arrow,
        Selection
    };

    class IcoHolder
    {
    private:
        static inline std::map<ico_t, palka::Texture> ico;

        palka::Texture create(std::string_view path)
        {
            palka::Texture t;
            t.LoadFromFile(path);
            return t;
        };
    public:

        IcoHolder() = default;

        void init()
        {
            ico[ico_t::Editor_Add] = create(".\\Data\\ico\\addico.png");
            ico[ico_t::Editor_Delete] = create(".\\Data\\ico\\trash.png");
            ico[ico_t::Folder] = create(".\\Data\\ico\\folder.png");
            ico[ico_t::Refresh] = create(".\\Data\\ico\\refresh.png");
            ico[ico_t::Magic] = create(".\\Data\\ico\\magic.png");
            ico[ico_t::Save] = create(".\\Data\\ico\\save.png");
            ico[ico_t::Create] = create(".\\Data\\ico\\create.png");
            ico[ico_t::Move] = create(".\\Data\\ico\\move.png");
            ico[ico_t::Hand] = create(".\\Data\\ico\\hand.png");
            ico[ico_t::File_img] = create(".\\Data\\ico\\file_img.png");
            ico[ico_t::File_txt] = create(".\\Data\\ico\\file_txt.png");
            ico[ico_t::Arrow] = create(".\\Data\\ico\\arrow.png");
            ico[ico_t::Selection] = create(".\\Data\\ico\\select.png");
        }

        static const auto getTexture(ico_t type)
        {
            return ico[type].textureID;
        }
    };
}

#endif //MAGPIE_ICOHOLDER_H
