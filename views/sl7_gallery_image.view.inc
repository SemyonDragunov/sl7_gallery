<?php

/**
 * @file
 * View: sl7_galley_image view.
 */

$view = new view();
$view->name = 'sl7_gallery_image';
$view->description = '';
$view->tag = 'default';
$view->base_table = 'sl7_gallery_image';
$view->human_name = 'Галерея - изображения';
$view->core = 7;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Master */
$handler = $view->new_display('default', 'Master', 'default');
$handler->display->display_options['use_ajax'] = TRUE;
$handler->display->display_options['use_more_always'] = FALSE;
$handler->display->display_options['use_more_text'] = 'ещё';
$handler->display->display_options['access']['type'] = 'perm';
$handler->display->display_options['access']['perm'] = 'sl7_gallery view';
$handler->display->display_options['cache']['type'] = 'time';
$handler->display->display_options['cache']['results_lifespan'] = '518400';
$handler->display->display_options['cache']['results_lifespan_custom'] = '0';
$handler->display->display_options['cache']['output_lifespan'] = '518400';
$handler->display->display_options['cache']['output_lifespan_custom'] = '0';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['exposed_form']['options']['submit_button'] = 'Применить';
$handler->display->display_options['exposed_form']['options']['reset_button_label'] = 'Сбросить';
$handler->display->display_options['exposed_form']['options']['exposed_sorts_label'] = 'Сортировать по';
$handler->display->display_options['exposed_form']['options']['sort_asc_label'] = 'По возрастанию';
$handler->display->display_options['exposed_form']['options']['sort_desc_label'] = 'По убыванию';
$handler->display->display_options['pager']['type'] = 'full';
$handler->display->display_options['pager']['options']['items_per_page'] = '16';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['pager']['options']['id'] = '0';
$handler->display->display_options['pager']['options']['quantity'] = '9';
$handler->display->display_options['pager']['options']['expose']['items_per_page_label'] = 'Элементов на страницу';
$handler->display->display_options['pager']['options']['expose']['items_per_page_options_all'] = TRUE;
$handler->display->display_options['pager']['options']['expose']['items_per_page_options_all_label'] = '- Все -';
$handler->display->display_options['pager']['options']['expose']['offset_label'] = 'Пропустить';
$handler->display->display_options['pager']['options']['tags']['first'] = '« первая';
$handler->display->display_options['pager']['options']['tags']['previous'] = '‹ предыдущая';
$handler->display->display_options['pager']['options']['tags']['next'] = 'следующая ›';
$handler->display->display_options['pager']['options']['tags']['last'] = 'последняя »';
$handler->display->display_options['style_plugin'] = 'default';
$handler->display->display_options['style_options']['default_row_class'] = FALSE;
$handler->display->display_options['style_options']['row_class_special'] = FALSE;
$handler->display->display_options['row_plugin'] = 'entity';
$handler->display->display_options['row_options']['view_mode'] = 'teaser';
/* Критерий сортировки: Галерея - изображение: Дата создания */
$handler->display->display_options['sorts']['created']['id'] = 'created';
$handler->display->display_options['sorts']['created']['table'] = 'sl7_gallery_image';
$handler->display->display_options['sorts']['created']['field'] = 'created';
$handler->display->display_options['sorts']['created']['order'] = 'DESC';

/* Display: Страница альбома */
$handler = $view->new_display('page', 'Страница альбома', 'full_page');
$handler->display->display_options['defaults']['empty'] = FALSE;
/* Поведение при отсутствии результатов: Глобальный: Text area */
$handler->display->display_options['empty']['area']['id'] = 'area';
$handler->display->display_options['empty']['area']['table'] = 'views';
$handler->display->display_options['empty']['area']['field'] = 'area';
$handler->display->display_options['empty']['area']['label'] = 'Альбом пуст';
$handler->display->display_options['empty']['area']['empty'] = TRUE;
$handler->display->display_options['empty']['area']['content'] = 'Альбом пуст.';
$handler->display->display_options['empty']['area']['format'] = 'filtered_html';
$handler->display->display_options['defaults']['arguments'] = FALSE;
/* Контекстный фильтр: Галерея - изображение: ID альбома */
$handler->display->display_options['arguments']['aid']['id'] = 'aid';
$handler->display->display_options['arguments']['aid']['table'] = 'sl7_gallery_image';
$handler->display->display_options['arguments']['aid']['field'] = 'aid';
$handler->display->display_options['arguments']['aid']['default_action'] = 'default';
$handler->display->display_options['arguments']['aid']['exception']['title_enable'] = TRUE;
$handler->display->display_options['arguments']['aid']['exception']['title'] = 'Все';
$handler->display->display_options['arguments']['aid']['title_enable'] = TRUE;
$handler->display->display_options['arguments']['aid']['title'] = '%1';
$handler->display->display_options['arguments']['aid']['default_argument_type'] = 'node';
$handler->display->display_options['arguments']['aid']['summary']['number_of_records'] = '0';
$handler->display->display_options['arguments']['aid']['summary']['format'] = 'default_summary';
$handler->display->display_options['arguments']['aid']['summary_options']['items_per_page'] = '25';
$handler->display->display_options['path'] = '-';