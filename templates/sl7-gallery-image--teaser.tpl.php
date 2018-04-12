<div class="<?php print $classes; ?>"<?php print $attributes; ?>>

  <?php print render($title_prefix); ?>

  <a class="colorbox colorbox-processed"
    <?php if (isset($element->sl7_gallery_body['und'][0])): ?>
      title="<?php print '' . truncate_utf8(strip_tags($element->sl7_gallery_body['und'][0]['value']), 20, TRUE, TRUE); ?>"
    <?php endif; ?>
     rel="gallery-all"
     href="<?php print file_create_url($element->sl7_gallery_image['und'][0]['uri']); ?>">

    <figure>
      <img id="img" src="<?php print image_style_url('sl7_gallery_image', $element->sl7_gallery_image['und'][0]['uri']); ?>" />
    </figure>

  </a>

  <?php print render($title_suffix); ?>

</div>