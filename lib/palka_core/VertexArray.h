//
// Created by neputin_94 on 11/14/2021.
//

#ifndef PALKA_VERTEXARRAY_H
#define PALKA_VERTEXARRAY_H

#include <type_traits>
#include "Vec2.h"
#include "Color.h"
#include "VertexArrayObject.h"


namespace palka
{
    template<typename T>
    concept VertexValue = std::is_base_of<T, Vec3f>::value ||
                          std::is_base_of<T, Color>::value ||
                          std::is_base_of<T, Vec2f>::value;

    class VertexArray
    {
    public:
        std::map<int, std::vector<unsigned char>> datas;
        std::vector<Vec3f> coords;
        std::vector<Color> vertex_color;
        std::vector<Vec2f> texture_coords;
        std::vector<Vec3f> normals;
        VertexArrayObject vao;
        VertexBufferObject vbo;

    public:
        VertexArray() = default;

        enum type
        {
            COORD = 0,
            TEX_COORD,
            NORMALS,
            VERT_COLOR
        };

        template<type t, class... Args>
        void addRaw(Args&& ... args)
        {
            if constexpr (t == type::VERT_COLOR)
                vertex_color.emplace_back(std::forward<Args>(args)...);
            if constexpr (t == type::COORD)
                coords.emplace_back(std::forward<Args>(args)...);
            if constexpr (t == type::NORMALS)
                normals.emplace_back(std::forward<Args>(args)...);
            if constexpr (t == type::TEX_COORD)
                texture_coords.emplace_back(std::forward<Args>(args)...);
        }

        template<type typ, class T>
        void setData(std::vector<T> data)
        {
            if constexpr (typ == type::VERT_COLOR)
                vertex_color = data;
            if constexpr (typ == type::COORD)
                coords = data;
            if constexpr (typ == type::NORMALS)
                normals = data;
            if constexpr (typ == type::TEX_COORD)
                texture_coords = data;
        }

        void pushToBuffer(bool stash)
        {

        }

        void createVAO(VertexArrayObject& vao, VertexBufferObject& vbo)
        {

            //vbo_coords, vbo_normals, vbo_vert_color, vbo_tex_coords
            vbo.create(coords.size() * sizeof(Vec3f) + normals.size() * sizeof(Vec3f));
//            vbo_coords.create(coords.size());
//            vbo_normals.create(normals.size());
//            vbo_vert_color.create(vertex_color.size());
//            vbo_tex_coords.create(texture_coords.size());

            vbo.setData(coords.size() * sizeof(Vec3f), &coords[0].x);
            vbo.setData(sizeof(Vec3f) * normals.size(), &normals[0].x, coords.size() * sizeof(Vec3f));
            vbo.setData(sizeof(Color) * vertex_color.size(), &vertex_color[0].r, coords.size() * sizeof(Vec3f) + sizeof(Vec3f) * normals.size());
            vbo.setData(sizeof(Vec2f) * texture_coords.size(), &texture_coords[0].x, coords.size() * sizeof(Vec3f) +
                                                                                     sizeof(Vec3f) * normals.size() + sizeof(Color) * vertex_color.size());

//            vbo_coords.setData(sizeof(Vec3f) * coords.size(), &coords[0].x);
//            vbo_normals.setData(sizeof(Vec3f) * normals.size(), &normals[0].x);
//            vbo_vert_color.setData(sizeof(Color) * vertex_color.size(), &vertex_color[0].r);
//            vbo_tex_coords.setData(sizeof(Vec2f) * texture_coords.size(), &texture_coords[0].x);

            vao.bind();
//            vao.setPointer_u(vbo_coords, 0, 3, GL_FLOAT, sizeof(Vec3f), 0);
//            vao.setPointer_u(vbo_vert_color, 1, 3, GL_FLOAT, sizeof(Color), 0);
//            vao.setPointer_u(vbo_tex_coords, 2, 3, GL_FLOAT, sizeof(Vec2f), 0);
//            vao.setPointer_u(vbo_normals, 3, 3, GL_FLOAT, sizeof(Vec3f), 0);
            vao.setPointer_u(vbo, 0, 3, GL_FLOAT, sizeof(Vec3f), 0);
            vao.setPointer_u(vbo, 1, 4, GL_FLOAT, sizeof(Color), 0);
            vao.setPointer_u(vbo, 2, 3, GL_FLOAT, sizeof(Vec2f), 0);
            vao.setPointer_u(vbo, 3, 3, GL_FLOAT, sizeof(Vec3f), 0);
            vao.unbind();
        }
    };

    class RawBufferStorage
    {
    public:
        struct buffer_data
        {
            BufferObject::BufferType type;
            std::vector<unsigned char> data;
            size_t bytesAdded = 0;
        };
        //using buffer_data = std::pair<BufferObject::BufferType, std::vector<unsigned char>>;
    private:
        std::map<int, buffer_data> datas_storage;
        ///VertexArrayObject vao;
        //VertexBufferObject vbo;

    public:
        RawBufferStorage() = default;

        RawBufferStorage(RawBufferStorage&& ot) noexcept
        {
            this->datas_storage = std::move(ot.datas_storage);
        }

        void setData(size_t id, buffer_data data)
        {
            datas_storage.emplace(std::make_pair(id, data));
        }

        template<class T>
        void addData(size_t id, const T& type, size_t repeat = 1)
        {
            addRawData(id, &type, static_cast<size_t>(sizeof(T)), repeat);
        }

        const auto& operator[](int index) const
        {
            return datas_storage.at(index);
        }

        void addRawData(size_t id, const void* data, size_t data_size, size_t repeat)
        {
            assert(datas_storage.contains(id) && "id not found");
            auto& buff = datas_storage[id];
            const auto bytesToAdd = data_size * repeat;
            const auto requiredCapacity = buff.bytesAdded + bytesToAdd;

            if(requiredCapacity > buff.data.capacity())
            {
                // auto newCapacity = buff.data.capacity() + requiredCapacity + 1;
                auto newCapacity = buff.data.capacity() * 2 + 1; // avoid 0 capacity
                while(newCapacity < requiredCapacity)
                    newCapacity *= 2;

                std::vector<unsigned char> newRawData;
                newRawData.reserve(newCapacity);
                memcpy(newRawData.data(), buff.data.data(), buff.bytesAdded);
                buff.data = std::move(newRawData);
                // buff.data = std::move(newRawData);
            }
            const auto& a1 = (*this)[1];
            for(size_t i = 0; i < repeat; i++)
            {
                memcpy(buff.data.data() + buff.bytesAdded, data, data_size);
                buff.bytesAdded += bytesToAdd;
            }

        }

        auto begin()
        {
            return datas_storage.begin();
        }

        auto end()
        {
            return datas_storage.end();
        }

        auto begin() const
        {
            return datas_storage.begin();
        }

        auto end() const
        {
            return datas_storage.end();
        }

        void createVAO(VertexArrayObject& vao, VertexBufferObject& vbo)
        {

        }
    };

    class VAOBindMap
    {
    public:
        //vao_id, type, bind_point, size_type, stride, offset, normilized
        struct vao_data
        {
            int vbo_id;
            int type;
            void* offset;
            int stride;
            bool normilized;
            int size_type;
        };
    private:
        std::map<int, vao_data> vao_map;
    public:
        void add(int bind_point, vao_data data)
        {
            vao_map.emplace(std::make_pair(bind_point, data));
        }

        auto begin()
        {
            return vao_map.begin();
        }

        auto end()
        {
            return vao_map.end();
        }

        auto begin() const
        {
            return vao_map.begin();
        }

        auto end() const
        {
            return vao_map.end();
        }
    };
}
#endif //PALKA_VERTEXARRAY_H
