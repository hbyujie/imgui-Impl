
#include <QApplication>
#include <QDockWidget>
#include <QFileDialog>
#include <QFileInfo>
#include <QToolBar>

#include "renderer_widget.h"
#include "src/assimp_reader.h"
#include "src/mainwindow.h"
#include "src/obj_reader.h"

#include "image_widget.h"
#include "simular_widget.h"

#include "geometry.h"

#include "common/math.h"

MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags flags) : QMainWindow(parent, flags)
{
    m_tool_bar = std::make_shared<QToolBar>();
    this->addToolBar(m_tool_bar.get());

    QAction *obj_reader = m_tool_bar->addAction(QString("Obj Reader"));
    connect(obj_reader, &QAction::triggered, this, &MainWindow::SlotObjRead);

    QAction *assimp_reader = m_tool_bar->addAction(QString("Assimp Reader"));
    connect(assimp_reader, &QAction::triggered, this, &MainWindow::SlotAssimpRead);

    // m_renderer_widget = std::make_shared<RendererWidget>();
    // this->setCentralWidget(m_renderer_widget.get());

    m_simular_widget.reset(new SimularWidget());
    this->setCentralWidget(m_simular_widget.get());

    m_image_dock = new QDockWidget("Image", this);
    this->addDockWidget(Qt::DockWidgetArea::RightDockWidgetArea, m_image_dock);

	m_float_dock = new QDockWidget("Float", this);
	this->addDockWidget(Qt::DockWidgetArea::AllDockWidgetAreas, m_float_dock);

    m_image_widget.reset(new ImageWidget());
    m_image_dock->setWidget(m_image_widget.get());

	connect(m_simular_widget.get(), &SimularWidget::SignalSendTextureID,
		m_image_widget.get(), &ImageWidget::SlotReceiveTexture);

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

    std::vector<uint32_t> indices;
    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);

    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(3);

    Geometry geometry;
    geometry.SetName("Test Triangle");

    geometry.SetMode("first", GL_TRIANGLES);
    geometry.SetVertexArray("first", positions);
    // geometry.SetIndiceArray("first", indices);

    geometry.UpdateBoundingBox();
    m_renderer_widget->AddGeometry("Test Triangle", geometry);
}

void MainWindow::SlotObjRead()
{
    QString file_name = QFileDialog::getOpenFileName(this, "Add Model", ".", "*.obj");
    if (!file_name.isEmpty())
    {
        QFileInfo file_info(file_name);

        OBJ::ObjReader reader(file_name.toStdString(), file_info.absolutePath().toStdString());
        auto &model = reader.GetModel();
        const auto &model_name = reader.GetModelName();

        Geometry geometry;
        geometry.SetName(model_name);

        const auto &face_size = model.faces.size();
        for (int i = 0; i < face_size; ++i)
        {
            const auto &vertex_size = model.faces[i].pos_indices.size();
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

            auto &name = model.faces[i].usemtl;
            auto &usemtl = model.usemtles[i];

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

        geometry.UpdateBoundingBox();
        m_renderer_widget->AddGeometry(model_name, geometry);
    }
}

void MainWindow::SlotAssimpRead()
{
    QString file_name = QFileDialog::getOpenFileName(this, "Add Model", ".", "*.obj");
    if (!file_name.isEmpty())
    {
        QFileInfo file_info(file_name);

        Assimp::AssimpReader reader(file_name.toStdString(), file_info.absolutePath().toStdString());
        auto &modeles = reader.GetModeles();
        const auto &model_name = reader.GetModelName();

        Geometry geometry;
        geometry.SetName(model_name);

        for (auto &model_map : modeles)
        {
            const auto &name = model_map.first;
            auto &data = model_map.second;

            TextureBuffer textures;
            textures.albedo.file_name = data.usemtl.map_kd;
            textures.normal.file_name = data.usemtl.map_bump;
            textures.metallic.file_name = data.usemtl.map_ks;

            Material material;
            material.ambient = data.usemtl.ka;
            material.diffuse = data.usemtl.kd;
            material.specular = data.usemtl.ks;
            material.shininess = data.usemtl.ns;

            geometry.SetMode(name, GL_TRIANGLES);
            geometry.SetVertexArray(name, data.positions);
            geometry.SetNormalArray(name, data.normals);
            geometry.SetTexcoordArray(name, data.texcoords);
            geometry.SetTangentArray(name, data.tangents);
            geometry.SetBitangentArray(name, data.bitangents);
            geometry.SetIndiceArray(name, data.usemtl.indices);
            geometry.SetTextures(name, textures);
            geometry.SetMaterial(name, material);
        }
        geometry.UpdateBoundingBox();
        m_renderer_widget->AddGeometry(model_name, geometry);
    }
}
