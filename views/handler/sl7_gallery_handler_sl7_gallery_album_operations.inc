<?php

class sl7_gallery_handler_sl7_gallery_album_operations extends views_handler_field {
  function construct() {
    parent::construct();
    $this->additional_fields['aid'] = 'aid';
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    $aid = $this->get_value($values, 'aid');
    $album = sl7_gallery_album_load($aid);
    $links = array();

    if (sl7_gallery_album_access('edit', $album)) {
      $links['edit'] = array(
        'title' => t('Edit'),
        'href'  => SL7_GALLERY_ALBUM_PATH . '/' . $aid . '/edit',
        'html'  => FALSE,
      );
      $links['edit']['query'] = drupal_get_destination();
    }

    if (sl7_gallery_album_access('delete', $album)) {
      $links['delete'] = array(
        'title' => t('Delete'),
        'href'  => SL7_GALLERY_ALBUM_PATH . '/' . $aid . '/delete',
        'html'  => FALSE,
      );
      $links['delete']['query'] = drupal_get_destination();
    }

    if (sl7_gallery_image_access('create')) {
      $links['add'] = array(
        'title' => 'Добавить изображение',
        'href'  => SL7_GALLERY_ALBUM_PATH . '/' . $aid . '/add',
        'html'  => FALSE,
      );
      $links['delete']['query'] = drupal_get_destination();
    }

    return theme('links__ctools_dropbutton', array(
      'links' => $links,
      'attributes' => array(
        'class' => array('links', 'inline', 'operations')
      )
    ));
  }
}