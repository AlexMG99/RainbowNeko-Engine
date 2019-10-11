#ifndef PRIMITIVE_H_
#define PRIMITIVE_H_

class Cube {
public:
	Cube() {};
	~Cube() {};

	void GenerateMesh();
	void Render();

public:
	par_shapes_mesh_s* cube_mesh = nullptr;

private:
	uint my_id = 0;
	uint my_indices = 0;
};

#endif // !PRIMITIVE_H_

