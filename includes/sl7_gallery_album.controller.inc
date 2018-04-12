<?php

/**
 * @author Semyon Dragunov <sam.dragunov@gmail.com>
 * https://github.com/SemyonDragunov
 */

/**
 * Class sl7_gallery_album entities.
 */
class SL7GalleryAlbum extends Entity {
  public function __construct($values = array()) {
    parent::__construct($values, 'sl7_gallery_album');
  }

  protected function defaultLabel() {
    return $this->label;
  }

  protected function defaultUri() {
    return array('path' => $this->realPath());
  }

  /**
   * Real path of album.
   */
  public function realPath() {
    return SL7_GALLERY_ALBUM_PATH . '/' . $this->identifier();
  }
}

/**
 * The Controller for sl7_gallery_album entities.
 */
class SL7GalleryAlbumController extends EntityAPIController {

  public function __construct($entityType) {
    parent::__construct($entityType);
  }

  public function create(array $values = array()) {
    global $user;

    $values += array(
      'aid' => '',
      'is_new' => TRUE,
      'label' => '',
      'uid' => $user->uid,
      'created' => REQUEST_TIME,
      'changed' => REQUEST_TIME,
    );

    return parent::create($values);
  }

  public function buildContent($entity, $view_mode = 'full', $langcode = NULL, $content = array()) {
    $content = parent::buildContent($entity, $view_mode, $langcode, $content);

    if ('teaser' == $view_mode) {
      $content['#contextual_links']['sl7_gallery'] = array(SL7_GALLERY_ALBUM_PATH, array($entity->aid));

      $content += array(
        '#theme'     => 'sl7_gallery_album__' . $view_mode,
        '#element'   => $entity,
        '#view_mode' => $view_mode,
        '#language'  => LANGUAGE_NONE,
      );
    }

    if ('full' == $view_mode) {
      $content['images']['#markup'] = views_embed_view('sl7_gallery_image', 'full_page', $entity->aid);
      $content['images']['#weight'] = 20;
    }

    return $content;
  }

  public function save($entity, DatabaseTransaction $transaction = NULL) {
    if (empty($entity->is_new)) $entity->changed = REQUEST_TIME;
    return parent::save($entity, $transaction);
  }
}

/**
 * Metadata controller for sl7_gallery_album entities.
 */
class SL7GalleryAlbumMetadataController extends EntityDefaultMetadataController {

  public function entityPropertyInfo() {
    $info = parent::entityPropertyInfo();

    $properties = &$info[$this->type]['properties'];

    $properties['label']['label'] = 'Название альбома';

    $properties['uid'] = array(
      'label' => 'Создатель',
      'description' => 'Имя пользователя создателя платежа.',
      'type' => 'user',
      'computed' => TRUE,
      'entity views field' => TRUE,
    );

    $properties['sl7_gallery_album_cover_url'] = array(
      'label' => 'Обложка',
      'description' => 'Обложка альбома.',
      'type' => 'text',
      'getter callback' => 'sl7_gallery_metadata_sl7_gallery_album_cover_url',
      'computed' => TRUE,
      'entity views field' => TRUE,
    );

    $properties['created']['type'] = 'date';
    $properties['created']['label'] = 'Дата создания';
    $properties['created']['description'] = 'Время создания платежа.';

    $properties['changed']['type'] = 'date';
    $properties['changed']['label'] = 'Последние изменение';
    $properties['changed']['description'] = 'Дата и время последнего изменения платежа.';

    return $info;
  }
}

/**
 * Views controller for sl7_event entities.
 */
class SL7GalleryAlbumViewsController extends EntityDefaultViewsController {

  public function views_data() {
    $data = parent::views_data();

    $data['sl7_gallery_album']['operations'] = array(
      'field' => array(
        'title' => 'Действия',
        'help' => 'Показывает все возможные операции.',
        'handler' => 'sl7_gallery_handler_sl7_gallery_album_operations',
      ),
    );

    return $data;
  }
}