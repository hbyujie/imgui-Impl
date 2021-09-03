
#include <QApplication>
#include <QDockWidget>
#include <QFileDialog>
#include <QFileInfo>
#include <QStatusBar>
#include <QToolBar>

#include "src/assimp_reader.h"
#include "src/mainwindow.h"
#include "src/obj_reader.h"

#include "image_widget.h"
#include "multi_images_widget.h"
#include "simular_scene.h"
#include "simular_widget.h"

#include "common/math.h"

MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags flags) : QMainWindow(parent, flags)
{
    m_tool_bar = std::make_shared<QToolBar>("ToolBar", this);
    this->addToolBar(m_tool_bar.get());

    QAction *obj_reader = m_tool_bar->addAction(QString("Obj Reader"));
    connect(obj_reader, &QAction::triggered, this, &MainWindow::SlotObjRead);

    QAction *assimp_reader = m_tool_bar->addAction(QString("Assimp Reader"));
    connect(assimp_reader, &QAction::triggered, this, &MainWindow::SlotAssimpRead);

    m_simular_widget.reset(new SimularWidget(this));
    this->setCentralWidget(m_simular_widget.get());

    m_debug_texture_widget.reset(new MultiImagesWidget(this));
    connect(m_simular_widget.get(), &SimularWidget::SendImage, m_debug_texture_widget.get(),
            &MultiImagesWidget::ReceiveImage);

    m_debug_dock_widget.reset(new QDockWidget("Debug Texture Widget", this));
    m_debug_dock_widget->setWidget(m_debug_texture_widget.get());
    this->addDockWidget(Qt::DockWidgetArea::RightDockWidgetArea, m_debug_dock_widget.get());

    this->setStatusBar(new QStatusBar());

    //   m_multi_images_dock_widget.reset(new QDockWidget("Multi Images", this));
    //   this->addDockWidget(Qt::DockWidgetArea::RightDockWidgetArea, m_multi_images_dock_widget.get());

    // m_display_manage_dock_widget.reset(new QDockWidget("Display", this));
    // this->addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, m_display_manage_dock_widget.get());
    // QWidget * test = new QWidget(m_display_manage_dock_widget.get());
    // test->setMinimumWidth(250);
    // m_display_manage_dock_widget->setWidget(test);

    //   m_multi_images_widget.reset(new MultiImagesWidget(this));
    //   m_multi_images_dock_widget->setWidget(m_multi_images_widget.get());

    //   connect(m_simular_widget.get(), &SimularWidget::SendImage, m_multi_images_widget.get(),
    //           &MultiImagesWidget::ReceiveImage);

    //   // m_image_widget1.reset(new ImageWidget(m_multi_images_widget.get()));
    //   // m_image_widget1->resize(400, 400);
    //   // m_image_widget1->SetImage("D:/imgui-openglwidget/data/textures/cyborg_diffuse.png");

    //   // m_image_widget2.reset(new ImageWidget(m_multi_images_widget.get()));
    //   // m_image_widget2->resize(400, 400);
    //   // m_image_widget2->SetImage("D:/imgui-openglwidget/data/textures/mars.png");
    //   // m_multi_images_widget->AddWidget(1, m_image_widget2);

    //   // m_image_widget3.reset(new ImageWidget(m_multi_images_widget.get()));
    //   // m_image_widget3->resize(400, 400);
    //   // m_image_widget3->SetImage("D:/imgui-openglwidget/data/textures/back.jpg");
    //   // m_multi_images_widget->AddWidget(2, m_image_widget3);

    ////QWidget * display_widget = new QWidget();
    ////QDockWidget * dock_widget = new QDockWidget("Display", this);
    ////dock_widget->setWidget(display_widget);
    ////this->addDockWidget(Qt::DockWidgetArea::BottomDockWidgetArea, dock_widget);

    //    m_multi_images_dock_widget->hide();
}

MainWindow::~MainWindow()
{
    m_simular_widget.reset();

    m_debug_texture_widget.reset();
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


}

void MainWindow::SlotObjRead()
{
    // Primitive primitive;
    // primitive.positions.push_back(glm::vec3(0.5f, 0.5f, 0.0f));
    // primitive.positions.push_back(glm::vec3(0.5f, -0.5f, 0.0f));
    // primitive.positions.push_back(glm::vec3(-0.5f, 0.5f, 0.0f));

    // primitive.indices.push_back(0);
    // primitive.indices.push_back(1);
    // primitive.indices.push_back(2);

    // primitive.line_width = 2.0f;
    // primitive.point_size = 5.0f;

    // primitive.mode = GL_TRIANGLES;

    // m_simular_widget->makeCurrent();
    // auto &m_simular_ptr = m_simular_widget->GetScenePtr();
    // m_simular_ptr->AddPrimitive("Line", primitive);

    // m_debug_texture_widget->AddWidget(0, ShaderPool::GetInstance()->GetShader("ColorImage"));
    // m_multi_images_widget->AddWidget(1, ShaderPool::GetInstance()->GetShader("ColorImage"));
    // m_multi_images_widget->AddWidget(2, ShaderPool::GetInstance()->GetShader("ColorImage"));

    QString file_name = QFileDialog::getOpenFileName(this, "Add Model", ".", "*.obj");
    if (!file_name.isEmpty())
    {
        QFileInfo file_info(file_name);

        OBJ::ObjReader reader(file_name.toStdString(), file_info.absolutePath().toStdString());
        auto &model = reader.GetModel();
        const auto &model_name = reader.GetModelName();
		
        const auto &face_size = model.faces.size();
        for (int i = 0; i < face_size; ++i)
        {
            const auto &vertex_size = model.faces[i].pos_indices.size();
            std::vector<glm::vec3> positions(vertex_size);
            std::vector<glm::vec3> normals(vertex_size);
            std::vector<glm::vec2> texcoords(vertex_size);
            std::vector<glm::vec3> tangents;
            std::vector<glm::vec3> bitangents;
            std::vector<uint32_t> indices(vertex_size);

            for (int j = 0; j < vertex_size; ++j)
            {
                positions[j] = model.positions[model.faces[i].pos_indices[j]];
                normals[j] = model.normals[model.faces[i].nor_indices[j]];
                texcoords[j] = model.texcoords[model.faces[i].tex_indices[j]];
                indices[j] = j;
            }
            OpenGL::Math::ComputeTangents(positions, texcoords, &tangents, &bitangents);

            auto &name = model.faces[i].usemtl;
            auto &usemtl = model.usemtles[i];

            Primitive primitive;
            positions.swap(primitive.positions);
            normals.swap(primitive.normals);
            texcoords.swap(primitive.texcoords);
            tangents.swap(primitive.tangents);
            bitangents.swap(primitive.bitangents);
            indices.swap(primitive.indices);

            primitive.albedo_file = usemtl.map_kd;
            primitive.normal_file = usemtl.map_bump;
            primitive.metallic_file = usemtl.map_ks;

			primitive.material.ambient = usemtl.ka;
			primitive.material.diffuse = usemtl.kd;
			primitive.material.specular = usemtl.ks;
			primitive.material.shininess = usemtl.ns;

            primitive.line_width = 2.0f;
            primitive.point_size = 5.0f;

            primitive.mode = GL_TRIANGLES;

            m_simular_widget->makeCurrent();
            auto &m_simular_ptr = m_simular_widget->GetScenePtr();
            m_simular_ptr->AddPrimitive(name, primitive);

            // TextureBuffer textures;
            // textures.albedo.file_name = usemtl.map_kd;
            // textures.normal.file_name = usemtl.map_bump;
            // textures.metallic.file_name = usemtl.map_ks;

            // Material material;
            // material.ambient = usemtl.ka;
            // material.diffuse = usemtl.kd;
            // material.specular = usemtl.ks;
            // material.shininess = usemtl.ns;

            // geometry.SetMode(name, GL_TRIANGLES);
            // geometry.SetVertexArray(name, positions);
            // geometry.SetNormalArray(name, normals);
            // geometry.SetTexcoordArray(name, texcoords);
            // geometry.SetTangentArray(name, tangents);
            // geometry.SetBitangentArray(name, bitangents);
            // geometry.SetTextures(name, textures);
            // geometry.SetMaterial(name, material);
        }

        // geometry.UpdateBoundingBox();
        // m_renderer_widget->AddGeometry(model_name, geometry);
    }
}

void MainWindow::SlotAssimpRead()
{
    // m_multi_images_widget->RemoveWidget(0);

    // QString file_name = QFileDialog::getOpenFileName(this, "Add Model", ".", "*.obj");
    // if (!file_name.isEmpty())
    //{
    //    QFileInfo file_info(file_name);

    //    Assimp::AssimpReader reader(file_name.toStdString(), file_info.absolutePath().toStdString());
    //    auto &modeles = reader.GetModeles();
    //    const auto &model_name = reader.GetModelName();

    //    Geometry geometry;
    //    geometry.SetName(model_name);

    //    for (auto &model_map : modeles)
    //    {
    //        const auto &name = model_map.first;
    //        auto &data = model_map.second;

    //        TextureBuffer textures;
    //        textures.albedo.file_name = data.usemtl.map_kd;
    //        textures.normal.file_name = data.usemtl.map_bump;
    //        textures.metallic.file_name = data.usemtl.map_ks;

    //        Material material;
    //        material.ambient = data.usemtl.ka;
    //        material.diffuse = data.usemtl.kd;
    //        material.specular = data.usemtl.ks;
    //        material.shininess = data.usemtl.ns;

    //        geometry.SetMode(name, GL_TRIANGLES);
    //        geometry.SetVertexArray(name, data.positions);
    //        geometry.SetNormalArray(name, data.normals);
    //        geometry.SetTexcoordArray(name, data.texcoords);
    //        geometry.SetTangentArray(name, data.tangents);
    //        geometry.SetBitangentArray(name, data.bitangents);
    //        geometry.SetIndiceArray(name, data.usemtl.indices);
    //        geometry.SetTextures(name, textures);
    //        geometry.SetMaterial(name, material);
    //    }
    //    geometry.UpdateBoundingBox();
    //    m_renderer_widget->AddGeometry(model_name, geometry);
    //}
}
