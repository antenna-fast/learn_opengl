#include <iostream>

// GLM: 3d math lib
#include <glm/glm.hpp>  
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

int main(int argc, char** argv){
    cout << argv[0] << endl;
    
    glm::vec4 position = glm::vec4(1, 0, 0, 1);

    glm::mat4 transMat = glm::mat4(1.0f);

    glm::vec4 res = transMat * position;

    cout << "res: " << res.x << " " << res.y << " " << res.z << endl;

    return 0;
}