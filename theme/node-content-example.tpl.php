  <div class="node<?php if ($sticky) { print " sticky"; } ?><?php if (!$status) { print " node-unpublished"; } ?>">
    <?php if ($picture) {
      print $picture;
    }?>
    <?php if ($page == 0) { ?><h2 class="title"><a href="<?php print $node_url?>"><?php print $title?></a></h2><?php }; ?>
    <span class="submitted"><?php print $submitted?></span>
    <span class="taxonomy"><?php print $terms?></span>
    <div class="content">
      Here's the body:
        <?php print $field_body[0]['view']; ?>
      
      And some footnotes:
      <ul>
        <?php
          foreach ($field_footnotes as $item) {
            print '<li>'. $item['view'] .'</li>';
          }
        ?>
      </ul>
    </div>
    <?php if ($links) { ?><div class="links">&raquo; <?php print $links?></div><?php }; ?>
  </div>
