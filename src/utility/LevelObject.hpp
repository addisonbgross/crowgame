class LevelObject {
public:
    std::string name;
    float x, y, z;
    float rotation;

    LevelObject(std::string name, float x, float y, float z, float rotation)
    {
        this->name = name;
        this->x = x;
        this->y = y;
        this->z = z;
        this->rotation = rotation;
    }
};
