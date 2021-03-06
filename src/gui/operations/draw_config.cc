/* 
 */

/*

    Copyright (C) 2014 Ferrero Andrea

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.


 */

/*

    These files are distributed with PhotoFlow - http://aferrero2707.github.io/PhotoFlow/

 */

#include "../../operations/draw.hh"

#include "draw_config.hh"


PF::DrawConfigDialog::DrawConfigDialog( PF::Layer* layer ):
  OperationConfigDialog( layer, "Draw" ),
  pen_color_label("Pen color:              "),
  bgd_color_label("Background color: "),
  pen_color_button( Gdk::RGBA("white") ),
  bgd_color_button( Gdk::RGBA("black") ),
  pen_size( this, "pen_size", "Pen size: ", 5, 0, 1000000, 1, 10, 1),
  pen_opacity( this, "pen_opacity", "Pen opacity: ", 100, 0, 100, 0.1, 1, 100)
{
  colorButtonsBox1.pack_start( bgd_color_label, Gtk::PACK_SHRINK );
  colorButtonsBox1.pack_start( bgd_color_button, Gtk::PACK_SHRINK );
  colorButtonsBox2.pack_start( pen_color_label, Gtk::PACK_SHRINK );
  colorButtonsBox2.pack_start( pen_color_button, Gtk::PACK_SHRINK );
  colorButtonsBox2.pack_start( pen_size, Gtk::PACK_SHRINK );
  controlsBox.pack_start( colorButtonsBox1 );
  controlsBox.pack_start( colorButtonsBox2 );
  penBox.pack_start( pen_opacity );
  controlsBox.pack_start( penBox );

  /*
  controlsBox.pack_start( pen_grey );
  controlsBox.pack_start( pen_R );
  controlsBox.pack_start( pen_G );
  controlsBox.pack_start( pen_B );
  
  controlsBox.pack_start( bgd_grey );
  controlsBox.pack_start( bgd_R );
  controlsBox.pack_start( bgd_G );
  controlsBox.pack_start( bgd_B );
  */  

  pen_color_button.signal_color_set().
    connect( sigc::mem_fun(this, &PF::DrawConfigDialog::on_pen_color_changed) );
  bgd_color_button.signal_color_set().
    connect( sigc::mem_fun(this, &PF::DrawConfigDialog::on_bgd_color_changed) );

  add_widget( controlsBox );
}



void PF::DrawConfigDialog::open()
{
  if( get_layer() && get_layer()->get_image() && 
      get_layer()->get_processor() &&
      get_layer()->get_processor()->get_par() ) {
    /*
    pen_grey.init();
    pen_R.init();
    pen_G.init();
    pen_B.init();

    bgd_grey.init();
    bgd_R.init();
    bgd_G.init();
    bgd_B.init();
    */
    pen_size.init();
    pen_opacity.init();
  }
  OperationConfigDialog::open();
}


void PF::DrawConfigDialog::on_pen_color_changed()
{
  // Pointer to the associated Layer object
  PF::Layer* layer = get_layer();
  if( !layer ) return;

  // First of all, the new stroke is recorded by the "master" operation
  PF::ProcessorBase* processor = layer->get_processor();
  if( !processor || !(processor->get_par()) ) return;
  
  PF::DrawPar* par = dynamic_cast<PF::DrawPar*>( processor->get_par() );
  if( !par ) return;
  
  par->get_pen_color().get().r = pen_color_button.get_rgba().get_red();
  par->get_pen_color().get().g = pen_color_button.get_rgba().get_green();
  par->get_pen_color().get().b = pen_color_button.get_rgba().get_blue();

  if( layer->get_image() )
    layer->get_image()->update();
}


void PF::DrawConfigDialog::on_bgd_color_changed()
{
  // Pointer to the associated Layer object
  PF::Layer* layer = get_layer();
  if( !layer ) return;

  // First of all, the new stroke is recorded by the "master" operation
  PF::ProcessorBase* processor = layer->get_processor();
  if( !processor || !(processor->get_par()) ) return;
  
  PF::DrawPar* par = dynamic_cast<PF::DrawPar*>( processor->get_par() );
  if( !par ) return;
  
  par->get_bgd_color().get().r = bgd_color_button.get_rgba().get_red();
  par->get_bgd_color().get().g = bgd_color_button.get_rgba().get_green();
  par->get_bgd_color().get().b = bgd_color_button.get_rgba().get_blue();

  if( layer->get_image() )
    layer->get_image()->update();
}


void PF::DrawConfigDialog::start_stroke()
{
  // Pointer to the associated Layer object
  PF::Layer* layer = get_layer();
  if( !layer ) return;

  // First of all, the new stroke is recorded by the "master" operation
  PF::ProcessorBase* processor = layer->get_processor();
  if( !processor || !(processor->get_par()) ) return;
  
  PF::DrawPar* par = dynamic_cast<PF::DrawPar*>( processor->get_par() );
  if( !par ) return;
  
  //par->start_stroke( get_pen_size(), get_pen_opacity() );
  par->start_stroke();


  // Then we loop over all the operations associated to the 
  // layer in the different views and we let them record the stroke as well
  PF::Image* image = layer->get_image();
  if( !image ) return;

  for( unsigned int vi = 0; vi < image->get_nviews(); vi++ ) {
    PF::View* view = image->get_view( vi );
    if( !view ) continue;

    PF::ViewNode* node = view->get_node( layer->get_id() );
    if( !node ) continue;

    processor = node->processor;
    if( !processor || !(processor->get_par()) ) continue;

    par = dynamic_cast<PF::DrawPar*>( processor->get_par() );
    if( !par ) continue;

    //par->start_stroke( get_pen_size(), get_pen_opacity() );
    par->start_stroke();
  }
}


void PF::DrawConfigDialog::draw_point( double x, double y )
{
  // Pointer to the associated Layer object
  PF::Layer* layer = get_layer();
  if( !layer ) return;

  // First of all, the new stroke is recorded by the "master" operation
  PF::ProcessorBase* processor = layer->get_processor();
  if( !processor || !(processor->get_par()) ) return;
  
  PF::DrawPar* par = dynamic_cast<PF::DrawPar*>( processor->get_par() );
  if( !par ) return;
  
  VipsRect update;
  par->draw_point( x, y, update );


  // Then we loop over all the operations associated to the 
  // layer in the different views and we let them record the stroke as well
  PF::Image* image = layer->get_image();
  if( !image ) return;

  for( unsigned int vi = 0; vi < image->get_nviews(); vi++ ) {
    PF::View* view = image->get_view( vi );
    if( !view ) continue;

    PF::ViewNode* node = view->get_node( layer->get_id() );
    if( !node ) continue;

    processor = node->processor;
    if( !processor || !(processor->get_par()) ) continue;

    par = dynamic_cast<PF::DrawPar*>( processor->get_par() );
    if( !par ) continue;

    par->draw_point( x, y, update );

    if( (update.width > 0) &&
	(update.height > 0) )
      view->update( update );
  }
}


void PF::DrawConfigDialog::pointer_press_event( int button, double x, double y, int mod_key )
{
  if( button != 1 ) return;
  start_stroke();
  draw_point( x, y );
}


void PF::DrawConfigDialog::pointer_release_event( int button, double x, double y, int mod_key )
{
  if( button != 1 ) return;
  //draw_point( x, y );
}


void PF::DrawConfigDialog::pointer_motion_event( int button, double x, double y, int mod_key )
{
  if( button != 1 ) return;
#ifndef NDEBUG
  std::cout<<"PF::DrawConfigDialog::pointer_motion_event() called."<<std::endl;
#endif
  draw_point( x, y );
}


