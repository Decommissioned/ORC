#include "resource_loader.h"

#include "error_codes.h"
#include "util.h"

#include "tiny_obj_loader.h"

#include <iostream>
#include <regex>
#include <stack>
#include <tuple>

extern int decodePNG(std::vector<unsigned char>& out_image, unsigned long& image_width, unsigned long& image_height, const unsigned char* in_png, size_t in_size, bool convert_to_rgba32);

namespace ORC_NAMESPACE
{

        MeshData ResourceLoader::LoadOBJ(const string& path)
        {
                MeshData data;
                vector<tinyobj::shape_t> shapes;
                vector<tinyobj::material_t> materials;
                string error = tinyobj::LoadObj(shapes, materials, (ORC_MESH_FOLDER_RELATIVE_PATH + path).c_str(), "./");

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

                vector<uint8> data = util::ReadFileBinary(ORC_TEXTURE_FOLDER_RELATIVE_PATH + path);
                
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

        string ResourceLoader::LoadShader(const string& path)
        {

                        // Load the actual file's content
                        static const string folder = ORC_SHADER_FOLDER_RELATIVE_PATH;
                        string content = util::ReadFileText(folder + path).c_str(); // Converting to a C string removes the null terminator, which is important since that could introduce a null terminator in the middle of the source code after replacing include files

                        // Search for include directives and replace them with their file's content
                        static const std::regex rgx_include(R"(^[ \t]*#pragma[ \t]+include[ \t]*\([ \t]*"(\S+))"R"("[ \t]*\)[ \t]*$)");
                        std::match_results<std::string::const_iterator> match;
                        std::stack<std::tuple<size_t, size_t, string>> replaces;

                        for (std::string::const_iterator start = content.begin();
                             std::regex_search(start, content.cend(), match, rgx_include);
                             start = match[0].second)
                        {
                                // Recursively load subsequently included files
                                // TODO: infinite recursion detection (i.e. circular include directives)
                                string directive = match[0];
                                replaces.emplace(match.position(), directive.length(), LoadShader(match[1]));
                        }

                        while (!replaces.empty())
                        {
                                auto tuple = replaces.top();
                                size_t offset = std::get<0>(tuple);
                                size_t length = std::get<1>(tuple);
                                string potato = std::get<2>(tuple);

                                content.replace(offset, length, potato);
                                replaces.pop();
                        }

                        return content;
        }

};