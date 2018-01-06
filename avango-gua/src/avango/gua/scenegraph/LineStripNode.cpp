#include <avango/gua/scenegraph/LineStripNode.hpp>

#if defined(AVANGO_DISTRIBUTION_SUPPORT)
#include <avango/gua/network/NetTransform.h>
#endif

#include <avango/Base.h>
#include <functional>

AV_FC_DEFINE(av::gua::LineStripNode);

AV_FIELD_DEFINE(av::gua::SFLineStripNode);
AV_FIELD_DEFINE(av::gua::MFLineStripNode);

av::gua::LineStripNode::LineStripNode(std::shared_ptr< ::gua::node::LineStripNode> guanode)
    : GeometryNode(guanode)
    , m_guaLineStripNode(guanode)
{
  AV_FC_ADD_ADAPTOR_FIELD(Geometry,
                        std::bind(&LineStripNode::getGeometryCB, this,std::placeholders::_1),
                        std::bind(&LineStripNode::setGeometryCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(Material,
                      std::bind(&LineStripNode::getMaterialCB, this,std::placeholders::_1),
                      std::bind(&LineStripNode::setMaterialCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(RenderToGBuffer,
                      std::bind(&LineStripNode::getRenderToGBufferCB, this,std::placeholders::_1),
                      std::bind(&LineStripNode::setRenderToGBufferCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(RenderToStencilBuffer,
                      std::bind(&LineStripNode::getRenderToStencilBufferCB, this,std::placeholders::_1),
                      std::bind(&LineStripNode::setRenderToStencilBufferCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(RenderVolumetric,
                      std::bind(&LineStripNode::getRenderVolumetricCB, this,std::placeholders::_1),
                      std::bind(&LineStripNode::setRenderVolumetricCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(RenderAsPoints,
                      std::bind(&LineStripNode::getRenderAsPointsCB, this,std::placeholders::_1),
                      std::bind(&LineStripNode::setRenderAsPointsCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(ScreenSpaceLineWidth,
                      std::bind(&LineStripNode::getScreenSpaceLineWidthCB, this,std::placeholders::_1),
                      std::bind(&LineStripNode::setScreenSpaceLineWidthCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(WasCreatedEmpty,
                      std::bind(&LineStripNode::getWasCreatedEmptyCB, this,std::placeholders::_1),
                      std::bind(&LineStripNode::setWasCreatedEmptyCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(TriggerUpdate,
                      std::bind(&LineStripNode::getTriggerUpdateCB, this,std::placeholders::_1),
                      std::bind(&LineStripNode::setTriggerUpdateCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(PrivateLineStripDataString,
                      std::bind(&LineStripNode::getPrivateLineStripDataStringCB, this,std::placeholders::_1),
                      std::bind(&LineStripNode::setPrivateLineStripDataStringCB, this,std::placeholders::_1));


  if (guanode->get_material()) {
    m_Material = av::Link<av::gua::Material>(new av::gua::Material(guanode->get_material()));
  }

}

void av::gua::LineStripNode::clearVertices() const
{
  m_guaLineStripNode->clear_vertices();
}

void av::gua::LineStripNode::popBackVertex() const
{
  m_guaLineStripNode->pop_back_vertex();
}

void av::gua::LineStripNode::popFrontVertex() const
{
  m_guaLineStripNode->pop_front_vertex();
}

void av::gua::LineStripNode::pushVertex(float x_pos, float y_pos, float z_pos,
                                        float col_r, float col_g, float col_b, float col_a,
                                        float thickness) const
{
  m_guaLineStripNode->push_vertex(x_pos, y_pos, z_pos,
                                  col_r, col_g, col_b, col_a,
                                  thickness);
}


#if defined(AVANGO_DISTRIBUTION_SUPPORT)

void av::gua::LineStripNode::on_distribute(av::gua::NetTransform& netNode) 
{
    GeometryNode::on_distribute(netNode);

    if (m_Material.isValid()) {
      m_Material->on_distribute(netNode);
    }

    netNode.distributeFieldContainer(m_Material);
}

void av::gua::LineStripNode::on_undistribute(av::gua::NetTransform& netNode) 
{
    GeometryNode::on_undistribute(netNode);

    if (m_Material.isValid()) {
      m_Material->on_undistribute(netNode);
    }

    netNode.undistributeFieldContainer(m_Material);
}
#endif

void
av::gua::LineStripNode::initClass()
{
  if (!isTypeInitialized()) {
    av::gua::GeometryNode::initClass();

    AV_FC_INIT(av::gua::GeometryNode, av::gua::LineStripNode, true);

    SFLineStripNode::initClass("av::gua::SFLineStripNode", "av::Field");
    MFLineStripNode::initClass("av::gua::MFLineStripNode", "av::Field");

    sClassTypeId.setDistributable(true);
  }
}

void
av::gua::LineStripNode::getGeometryCB(const SFString::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaLineStripNode->get_geometry_description();
}

void
av::gua::LineStripNode::setGeometryCB(const SFString::SetValueEvent& event)
{
  m_guaLineStripNode->set_geometry_description(event.getValue());
}

void
av::gua::LineStripNode::getMaterialCB(const SFMaterial::GetValueEvent& event)
{
  if (m_Material.isValid()) {
    *(event.getValuePtr()) = m_Material;
  }
}

void
av::gua::LineStripNode::setMaterialCB(const SFMaterial::SetValueEvent& event)
{
  if (event.getValue().isValid()) {
    m_Material = event.getValue();
    m_guaLineStripNode->set_material(m_Material->getGuaMaterial());
  }
}

void
av::gua::LineStripNode::getRenderToGBufferCB(const SFBool::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaLineStripNode->get_render_to_gbuffer();
}

void
av::gua::LineStripNode::setRenderToGBufferCB(const SFBool::SetValueEvent& event)
{
  m_guaLineStripNode->set_render_to_gbuffer(event.getValue());
}

void
av::gua::LineStripNode::getRenderToStencilBufferCB(const SFBool::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaLineStripNode->get_render_to_stencil_buffer();
}

void
av::gua::LineStripNode::setRenderToStencilBufferCB(const SFBool::SetValueEvent& event)
{
  m_guaLineStripNode->set_render_to_stencil_buffer(event.getValue());
}

void av::gua::LineStripNode::getRenderVolumetricCB(const SFBool::GetValueEvent& event)
{
  //*(event.getValuePtr()) = m_guaLineStripNode->get_render_volumetric();
}

void av::gua::LineStripNode::setRenderVolumetricCB(const SFBool::SetValueEvent& event)
{
  m_guaLineStripNode->set_render_volumetric(event.getValue());
}

void av::gua::LineStripNode::getRenderAsPointsCB(const SFBool::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaLineStripNode->get_render_vertices_as_points();
}

void av::gua::LineStripNode::setRenderAsPointsCB(const SFBool::SetValueEvent& event)
{
  m_guaLineStripNode->set_render_vertices_as_points(event.getValue());
}

void av::gua::LineStripNode::getScreenSpaceLineWidthCB(const SFFloat::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaLineStripNode->get_screen_space_line_width();
}

void av::gua::LineStripNode::setScreenSpaceLineWidthCB(const SFFloat::SetValueEvent& event)
{
  m_guaLineStripNode->set_screen_space_line_width(event.getValue());
}

void av::gua::LineStripNode::getWasCreatedEmptyCB(const SFBool::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaLineStripNode->get_was_created_empty();
}

void av::gua::LineStripNode::setWasCreatedEmptyCB(const SFBool::SetValueEvent& event)
{
  m_guaLineStripNode->set_was_created_empty(event.getValue());
}

void av::gua::LineStripNode::getTriggerUpdateCB(const SFBool::GetValueEvent& event)
{
  std::cout << "WOULD HAVE CALLED GET TRIGGER UPDATE\n";
  *(event.getValuePtr()) = m_guaLineStripNode->get_trigger_update();
}

void av::gua::LineStripNode::setTriggerUpdateCB(const SFBool::SetValueEvent& event)
{
  std::cout << "CALLED CALLED SET TRIGGER UPDATE with value: " << event.getValue() << "\n";

  role_server_client_unidentified = event.getValue() == 1 ? 0 : 1;
  if(0 == role_server_client_unidentified) {
    m_guaLineStripNode->set_trigger_update(!event.getValue());

    std::string compiled_buffer_string;
    
    m_guaLineStripNode->compile_buffer_string(compiled_buffer_string);

    std::cout << "COMILED BUFFER SIZE FINAL: " << compiled_buffer_string.size() << "\n";

    privateLineStripData = compiled_buffer_string;// + std::string("+++");//std::string("MyTestData123456789");
  }

}

void av::gua::LineStripNode::getPrivateLineStripDataStringCB(const SFString::GetValueEvent& event)
{
  //std::cout << "CALLED LSDATA GETTER " << privateLineStripData << "\n";

  std::cout << "BEFORE WRITING IT ON THE SERVER SIZE, THE STRING STILL CONTAINS: " << privateLineStripData.size() << " BYTES\n";

  *(event.getValuePtr()) = privateLineStripData;
}

void av::gua::LineStripNode::setPrivateLineStripDataStringCB(const SFString::SetValueEvent& event)
{

  if(1 == role_server_client_unidentified) {
    std::string received = event.getValue(); 
    //std::cout << "CALLED LSDATA SETTER WITH: " << received << "\n";
    std::cout << "it contains: " << received.size() << " Bytes\n";

    uint64_t num_vertices_written = 0;

    uint64_t read_offset = 0;
    memcpy(&num_vertices_written, &received[0], sizeof(num_vertices_written) );

    std::cout << "RECEIVED " << num_vertices_written << " VERTICES\n";

    //if()
      std::vector<::gua::math::vec3f> positions(num_vertices_written);
      std::vector<::gua::math::vec4f> colors(num_vertices_written);
      std::vector<float> thicknesses(num_vertices_written);


      read_offset += sizeof(num_vertices_written);
      memcpy(&positions[0], &received[read_offset], num_vertices_written*sizeof(::gua::math::vec3f));
      read_offset +=  num_vertices_written*sizeof(::gua::math::vec3f);
      memcpy(&colors[0], &received[read_offset], num_vertices_written*sizeof(::gua::math::vec4f));
      read_offset +=  num_vertices_written*sizeof(::gua::math::vec4f);
      memcpy(&thicknesses[0], &received[read_offset], num_vertices_written*sizeof(float));

      m_guaLineStripNode->clear_vertices();


      for(uint64_t vertex_idx = 0; vertex_idx < num_vertices_written; ++vertex_idx) {

        auto const& current_pos = positions[vertex_idx];
        auto const& current_col = colors[vertex_idx];
        auto const& current_thick = thicknesses[vertex_idx];
        //std::cout << "Pushing vertex: " << pos_entry[0] << " " << pos_entry[1] << " " << pos_entry[2] << "\n\n";
        m_guaLineStripNode->push_vertex(current_pos[0], current_pos[1], current_pos[2], 
                                        current_col[0], current_col[1], current_col[2], current_col[3], 
                                        current_thick);
      }
  //m_guaLineStripNode->set_was_created_empty(event.getValue());
  }
}

std::shared_ptr< ::gua::node::LineStripNode>
av::gua::LineStripNode::getGuaLineStripNode() const {
  return m_guaLineStripNode;
}

void 
av::gua::LineStripNode::submitVertices() {
  PrivateLineStripDataString.setValue("");
}