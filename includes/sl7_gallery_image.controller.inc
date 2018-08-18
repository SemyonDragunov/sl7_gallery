<?php

/**
 * Class sl7_gallery_image entities.
 */
class SL7GalleryImage extends Entity {
  public function __construct($values = array()) {
    parent::__construct($values, 'sl7_gallery_image');
  }

  protected function defaultLabel() {
    return sl7_gallery_image_title($this);
  }

  protected function defaultUri() {
    return array('path' => $this->realPath());
  }

  public function realPath() {
    return SL7_GALLERY_IMAGE_PATH . '/' . $this->identifier();
  }
}

/**
 * The Controller for sl7_gallery_image entities.
 */
class SL7GalleryImageController extends EntityAPIController {

  public function __construct($entityType) {
    parent::__construct($entityType);
  }

  public function create(array $values = array()) {
    global $user;

    $values += array(
      'iid' => '',
      'is_new' => TRUE,
      'cover' => 0,
      'aid' => '',
      'uid' => $user->uid,
      'created' => REQUEST_TIME,
      'changed' => REQUEST_TIME,
    );

    return parent::create($values);
  }

  public function buildContent($entity, $view_mode = 'full', $langcode = NULL, $content = array()) {
    $content = parent::buildContent($entity, $view_mode, $langcode, $content);

    field_attach_prepare_view('sl7_gallery_image', array($entity->iid => $entity), $view_mode, $langcode);
    entity_prepare_view('sl7_gallery_image', array($entity->iid => $entity), $langcode);
    $content += field_attach_view('sl7_gallery_image', $entity, $view_mode, $langcode);

    if ('teaser' == $view_mode) {
      $content['#contextual_links']['sl7_gallery'] = array(SL7_GALLERY_IMAGE_PATH, array($entity->iid));

      $content += array(
        '#theme'     => 'sl7_gallery_image__' . $view_mode,
        '#element'   => $entity,
        '#view_mode' => $view_mode,
        '#language'  => LANGUAGE_NONE,
      );
    }

    return $content;
  }

  public function save($entity, DatabaseTransaction $transaction = NULL) {
    if (empty($entity->is_new)) $entity->changed = REQUEST_TIME;
    return parent::save($entity, $transaction);
  }
}

/**
 * Metadata controller for sl7_gallery_image entities.
 */
class SL7GalleryImageMetadataController extends EntityDefaultMetadataController {

  public function entityPropertyInfo() {
    $info = parent::entityPropertyInfo();

    $properties = &$info[$this->type]['properties'];

    $properties['iid']['type'] = 'integer';
    $properties['iid']['label'] = 'ID изображения';
    $properties['iid']['description'] = 'ID изображения (iid).';

    $properties['uid'] = array(
      'label' => 'Создатель',
      'description' => 'Имя пользователя создателя платежа.',
      'type' => 'user',
      'computed' => TRUE,
      'entity views field' => TRUE,
    );

    $properties['aid']['type'] = 'integer';
    $properties['aid']['label'] = 'ID альбома';
    $properties['aid']['description'] = 'ID альбома этого изображения.';

    $properties['created']['type'] = 'date';
    $properties['created']['label'] = 'Дата создания';
    $properties['created']['description'] = 'Время создания платежа.';

    $properties['changed']['type'] = 'date';
    $properties['changed']['label'] = 'Последние изменение';
    $properties['changed']['description'] = 'Дата и время последнего изменения платежа.';

    return $info;
  }
}