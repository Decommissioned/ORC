#include "resource_loader.h"

#include "error_codes.h"
#include "util.h"

#include "tiny_obj_loader.h"

extern int decodePNG(std::vector<unsigned char>& out_image, unsigned long& image_width, unsigned long& image_height, const unsigned char* in_png, size_t in_size, bool convert_to_rgba32);

static const string img_folder = "../image/";
static const string mesh_folder = "../mesh/";

#include <iostream>

namespace ORC_NAMESPACE
{

        MeshData ResourceLoader::LoadOBJ(const string& path)
        {
                MeshData data;
                vector<tinyobj::shape_t> shapes;
                vector<tinyobj::material_t> materials;
                string error = tinyobj::LoadObj(shapes, materials, (mesh_folder + path).c_str(), "./");

                if (!error.empty())
                {
                        std::cerr << "Error loading .OBJ mesh " << path << ": " << error << std::endl;
                        throw Error::IO_LOAD_OBJ;
                }

                if (shapes.size() > 1)
                {
                        std::cerr << "Multiple meshes are not supported yet .OBJ " << path << std::endl;
                        throw Error::IO_LOAD_OBJ;
                }

                data.positions = std::move(shapes[0].mesh.positions);
                data.normals = std::move(shapes[0].mesh.normals);
                data.indices = std::move(shapes[0].mesh.indices);
                data.uvs = std::move(shapes[0].mesh.texcoords);

                return data;
        }

        ImageData ResourceLoader::LoadPNG(const string& path)
        {
                ImageData img;

                vector<uint8> data = util::ReadFileBinary(img_folder + path);
                
                unsigned long width, height;

                if (decodePNG(img.data, width, height, &data[0], data.size(), true) != 0)
                {
                        std::cerr << "Error loading .PNG image " << path << std::endl;
                        throw Error::IO_LOAD_PNG;
                }

                img.width = static_cast<uint32>(width);
                img.height = static_cast<uint32>(height);

                return img;
        }

};