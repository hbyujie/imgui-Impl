
#include <QApplication>
#include <QFileInfo>
#include <QFileDialog>
#include <QToolBar>

#include "src/mainwindow.h"
#include "src/obj_reader.h"
#include "renderer_widget.h"
#include "geometry.h"

#include "common/math.h"

MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags flags) : QMainWindow(parent, flags)
{
    m_tool_bar = std::make_shared<QToolBar>();
    this->addToolBar(m_tool_bar.get());

    QAction *add_model = m_tool_bar->addAction(QString("Add Model"));
    connect(add_model, &QAction::triggered, this, &MainWindow::SlotAddModel);

	m_renderer_widget = std::make_shared<RendererWidget>();
    this->setCentralWidget(m_renderer_widget.get());

}

MainWindow::~MainWindow()
{
}

void MainWindow::InitScene()
{
	std::vector<glm::vec3> positions;
	positions.push_back(glm::vec3(0, 0.5, 0));
	positions.push_back(glm::vec3(0.5, -0.5, 0));
	positions.push_back(glm::vec3(-0.5, -0.5, 0));

	positions.push_back(glm::vec3(0.5, 0.5, 0));
	positions.push_back(glm::vec3(1.0, -0.5, 0));
	positions.push_back(glm::vec3(0.0, -0.5, 0));

	//positions.push_back(glm::vec3(-100, 100, 0));
	//positions.push_back(glm::vec3(0, 100, 0));
	//positions.push_back(glm::vec3(100, 100, 0));
	//positions.push_back(glm::vec3(-100, 0, 0));
	//positions.push_back(glm::vec3(0, 0, 0));
	//positions.push_back(glm::vec3(100, 0, 0));
	//positions.push_back(glm::vec3(-100, -100, 0));
	//positions.push_back(glm::vec3(0, -100, 0));
	//positions.push_back(glm::vec3(100, -100, 0));

	Geometry geometry;
	geometry.SetName("Test Triangle");
	geometry.UpdateBoundingBox(positions);

	geometry.SetMode("first", GL_TRIANGLES);
	geometry.SetVertexArray("first", positions);

	m_renderer_widget->AddGeometry("Test Triangle", geometry);
}

void MainWindow::SlotAddModel()
{

	//InitScene();

    QString file_name = QFileDialog::getOpenFileName(this, "Add Model", ".", "*.obj");
    if (!file_name.isEmpty())
    {
		QFileInfo file_info(file_name);
		
		OBJ::ObjReader reader(file_name.toStdString(), file_info.absolutePath().toStdString());
		auto& model = reader.GetModel(); 

		Geometry geometry;
		geometry.SetName(model.mtllib);
		geometry.UpdateBoundingBox(model.positions);

		if (reader.IsSharedIndices())
		{
			// °þÀë vertex + indices, drawelement
			const auto& face_size = model.faces.size();
			for (int i = 0; i < face_size; ++i)
			{
				auto& name = model.faces[i].usemtl;
				auto& indices = model.faces[i].pos_indices;
				//auto& material = model.materials[i];
			}

		}
		else
		{
			// °þÀë vertex, drawarray
			const auto& face_size = model.faces.size();
			for (int i = 0; i < face_size; ++i)
			{
				const auto& vertex_size = model.faces[i].pos_indices.size();
				std::vector<glm::vec3> positions(vertex_size);
				std::vector<glm::vec3> normals(vertex_size);
				std::vector<glm::vec2> texcoords(vertex_size);
				std::vector<glm::vec3> tangents;
				std::vector<glm::vec3> bitangents;
				for (int j = 0; j < vertex_size; ++j)
				{
					positions[j] = model.positions[model.faces[i].pos_indices[j]];
					normals[j] = model.normals[model.faces[i].nor_indices[j]];
					texcoords[j] = model.texcoords[model.faces[i].tex_indices[j]];
				}
				OpenGL::Math::ComputeTangents(positions, texcoords, &tangents, &bitangents);

				auto& name = model.faces[i].usemtl;
				auto& usemtl = model.usemtles[i];

				TextureBuffer textures;
				textures.albedo.file_name = usemtl.map_kd;
				textures.normal.file_name = usemtl.map_bump;
				textures.metallic.file_name = usemtl.map_ks;
				
				Material material;
				material.ambient = usemtl.ka;
				material.diffuse = usemtl.kd;
				material.specular = usemtl.ks;
				material.shininess = usemtl.ns;

				geometry.SetMode(name, GL_TRIANGLES);
				geometry.SetVertexArray(name, positions);
				geometry.SetNormalArray(name, normals);
				geometry.SetTexcoordArray(name, texcoords);
				geometry.SetTangentArray(name, tangents);
				geometry.SetBitangentArray(name, bitangents);
				geometry.SetTextures(name, textures);
				geometry.SetMaterial(name, material);
			}
		}
		m_renderer_widget->AddGeometry(model.mtllib, geometry);
	}
}
