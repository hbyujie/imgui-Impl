
#include "opengl/model_reader.h"
#include "opengl/gl_mesh.h"
#include "opengl/obj_reader.h"

namespace File
{
std::string suffix(const char *path)
{
    std::string str(path);
    char ch = (char)'.';
    size_t pos = str.find_last_of(ch);

    return str.substr(pos + 1, str.length() - pos - 1);
}
std::string absolutePath(const char *path)
{
    std::string str(path);
    char ch = (char)'//';
    size_t pos = str.find_last_of(ch);

    return str.substr(0, pos + 1);
}
std::string getFileName(const char *path)
{
    std::string str(path);
    char ch1 = (char)'//';
    size_t pos1 = str.find_last_of(ch1);

    char ch2 = (char)'.';
    size_t pos2 = str.find_last_of(ch2);

    return str.substr(pos1 + 1, pos2 - pos1 - 1);
}
std::string getCurrentDirFileName(const char *path)
{
    std::string str(path);
    char ch1 = (char)'//';
    size_t pos1 = str.find_last_of(ch1);

    char ch2 = (char)'.';
    size_t pos2 = str.find_last_of(ch2);

    return str.substr(pos1 + 1, strlen(path));
}
std::string getFullFileName(const char *path)
{
    std::string str(path);
    char ch1 = (char)'//';
    size_t pos = str.find_last_of(ch1);

    return str.substr(pos + 1, str.length() - pos - 1);
}
std::string toUpper(const char *str)
{
    std::string reStr = std::string(str);

    for (size_t i = 0; i < strlen(str); ++i)
    {
        if (str[i] >= 'a' && str[i] <= 'z')
        {
            reStr[i] += 'A' - 'a';
        }
    }

    return reStr;
}
} // namespace File

ModelReader::ModelReader(const std::string &filename, const std::shared_ptr<GLMesh> mesh)
{
    std::string suffix = File::toUpper(File::suffix(filename.c_str()).c_str());
    std::string directory = File::absolutePath(filename.c_str());
	mesh->SetName(File::getFileName(filename.c_str()));

    if (suffix.compare("OBJ") == 0)
    {
        ObjReader reader(filename, directory);
        reader.GetMesh(mesh);
    }
}