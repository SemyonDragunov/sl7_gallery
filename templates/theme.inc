<?php

function template_preprocess_sl7_gallery_image__teaser(&$variables) {
  $entity = sl7_gallery_image_load($variables['element']->iid);
  $element = entity_view('sl7_gallery_image', array($entity), 'teaser');
  $element = $element['sl7_gallery_image'][$variables['element']->iid];

  $variables['title_suffix'] = array(
    'contextual_links' => array(
      '#type' => 'contextual_links',
      '#contextual_links' => $element['#contextual_links'],
      '#element' => $element,
    ),
  );

  $variables['classes_array'][] = 'sl7-gallery-image';
  $variables['classes_array'][] = 'teaser';
  $variables['classes_array'][] = 'contextual-links-region';

  drupal_add_css(drupal_get_path('module', 'sl7_gallery') . '/css/sl7_gallery.css');
}

function template_preprocess_sl7_gallery_album__teaser(&$variables) {
  $entity = sl7_gallery_album_load($variables['element']->aid);
  $element = entity_view('sl7_gallery_album', array($entity), 'teaser');
  $element = $element['sl7_gallery_album'][$variables['element']->aid];

  $variables['title'] = sl7_gallery_album_title($entity);
  $variables['url'] = url($entity->realPath());

  if (is_object($cover = sl7_gallery_album_get_cover($entity->aid))) {
    $variables['cover'] = image_style_url('sl7_gallery_album', $cover->sl7_gallery_image['und'][0]['uri']);
  }
  else {
    $variables['cover'] = $cover;
  }

  $variables['title_suffix'] = array(
    'contextual_links' => array(
      '#type' => 'contextual_links',
      '#contextual_links' => $element['#contextual_links'],
      '#element' => $element,
    ),
  );

  $variables['classes_array'][] = 'sl7-gallery-album';
  $variables['classes_array'][] = 'teaser';
  $variables['classes_array'][] = 'contextual-links-region';

  drupal_add_css(drupal_get_path('module', 'sl7_gallery') . '/css/sl7_gallery.css');
}