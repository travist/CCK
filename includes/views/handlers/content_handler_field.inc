<?php
// $Id$

/**
 * The subclass adds basic field and formatter info,
 * for field-specific subclasses to use if they need to.
 *
 * Fields could extend this class if they want field and formatter handling
 * but don't want the multiple value grouping options created by
 * content_handler_field_multiple.
 */
class content_handler_field extends views_handler_field_node {
  var $content_field;

  function construct() {
    parent::construct();
    $this->content_field = content_fields($this->definition['content_field_name']);
  }

  function options(&$options) {
    parent::options($options);
    $field = $this->content_field;

    // Override views_handler_field_node's default label
    $options['label'] = '';
    $options['label_type'] = 'widget';
    $options['format'] = 'default';
  }

  /**
   * Provide formatter option.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    // TODO: do we want the 'link to node' checkbox ?
    // That's usually formatters business...

    $field = $this->content_field;
    $options = $this->options;

    $field_types = _content_field_types();
    $formatters = array();
    if (is_array($field_types[$field['type']]['formatters'])) {
      foreach ($field_types[$field['type']]['formatters'] as $name => $info) {
        $formatters[$name] = $info['label'];
      }
    }
    $form['format'] = array(
      '#title' => t('Format'),
      '#type' => 'select',
      '#options' => $formatters,
      '#required' => TRUE,
      '#default_value' => $options['format'],
      '#weight' => 0,
    );

    $form['label_type'] = array(
      '#title' => t('Label'),
      '#type' => 'radios',
      '#options' => array(
        'none' => t('None'),
        'widget' => t('Widget label (@label)', array('@label' => $field['widget']['label'])),
        'custom' => t('Custom'),
      ),
      '#default_value' => $options['label_type'],
      '#weight' => 1,
    );
    $form['label'] = array(
      '#title' => t('Custom label'),
      '#type' => 'textfield',
      '#default_value' => $options['label'],
      '#process' => array('views_process_dependency'),
      '#dependency' => array('radio:options[label_type]' => array('custom')),
      '#weight' => 2,
     );
  }

  function label() {
    $field = $this->content_field;
    switch ($this->options['label_type']) {
      case 'none':
        return '';
      case 'widget':
        return t($field['widget']['label']);
      default:
        return $this->options['label'];
    }
  }

  function options_validate($form, &$form_state) { }

  /**
   * Provide text for the administrative summary
   */
  function admin_summary() {
    // Display the formatter name.
    $field = $this->content_field;
    $field_types = _content_field_types();
    if (isset($field_types[$field['type']]['formatters'][$this->options['format']])) {
      return t($field_types[$field['type']]['formatters'][$this->options['format']]['label']);
    }
  }

  function render($values) {
    $field = $this->content_field;
    $options = $this->options;
    $db_info = content_database_info($field);

    // $values will be used as a fake $node object;
    // we provide a build_mode for rendering.
    // TODO: we can stick any value in there -
    // what would make most sense ?  row_style ?
    $values->build_mode = 'views';

    $item = array();
    foreach ($db_info['columns'] as $column => $attributes) {
      $item[$column] = $values->{$this->table_alias .'_'. $attributes['column']};
    }
    $item['#delta'] = !empty($values->{$this->table_alias . '_delta'}) ? $values->{$this->table_alias . '_delta'} : 0;
    return $this->render_link(content_format($field, $item, $options['format'], $values), $values);
  }

  function query() {
    // Many of our formatters need the node type name, so let's be
    // sure it's in the query.
    $this->view->query->add_field('node', 'type');
    parent::query();
  }
}